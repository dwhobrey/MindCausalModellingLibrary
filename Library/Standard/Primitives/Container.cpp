#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Point.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Property.h"
#include "ThreadLocalVariable.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Model.h"
#include "CoordinateOperations.h"
#include "CoordinateNames.h"
#include "Geometry.h"
#include "PipeGeometry.h"
#include "UnivariateDistribution.h"
#include "ProducerModes.h"
#include "ProducerListener.h"
#include "Producer.h"
#include "Director.h"
#include "IdentifierEnumerator.h"

namespace Plato {

    const ClassTypeInfo* Container::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Container::PropertyTypes = NULL;

    void Container::Initializer() {
        TypeInfo = new ClassTypeInfo("Container",2,1,NULL,Property::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(Property::TypeInfo,NULL);
    }

    void Container::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

#pragma region // Constructors.
    Container::Container(const ClassTypeInfo::HashSet& propertyTypes, Container* creator, Container* parent, 
            Identifier& identifier, PropertyScopesEnum scope)
        : Property(creator, parent, identifier, scope) {
        Flags.IsContainer = true;
        OrderedProperties = new vector<Property*>();
        HashedProperties = new HashTable();
        mCategorisedProperties = new HashMap();
        ClassTypeInfo::HashSet::const_iterator itr;
        for(itr=propertyTypes.begin();itr!=propertyTypes.end();++itr) {
            (*mCategorisedProperties)[*itr] = new vector<Property*>();
        }
        mCategoryCache = new HashMap();
        mProducerListener = NULL;
        mLastOrder = 0;
        mIsClone = false;
        mDoDelete = true;
        IsUnrestricted = true;
        IsComposite = true;
        ConfigurePhase = ConfigurePhases::Unconfigured;
        // Give elements a default geometry - in case user forgets.
        ContainerGeometry = new PipeGeometry(UnivariateDistribution::UnboundedCapacity);
        ContainerGeometry->GeometryContainer = this;
        // Identity.Position = NULL; // DEBUG: Why setting this to NULL?
        ContainerModel = &Model::Current();
    }

    Container::~Container() {
        if(mIsClone) return;
        Clear(mDoDelete);
        Destroy();
    }

    void Container::Destroy() {
        delete OrderedProperties;
        delete HashedProperties;
        HashMap::iterator itr;
        for(itr=mCategorisedProperties->begin();itr!=mCategorisedProperties->end();++itr) {
            delete itr->second;
            itr->second = NULL;
        }
        delete mCategorisedProperties;
        delete mCategoryCache;
        delete ContainerGeometry;
    }

    void Container::CloneContents(Container& cloneContainer) const {
        cloneContainer.Destroy();      
        cloneContainer.OrderedProperties = OrderedProperties;
        cloneContainer.HashedProperties = HashedProperties;
        cloneContainer.mCategorisedProperties = mCategorisedProperties;
        cloneContainer.mCategoryCache = mCategoryCache;
        cloneContainer.mProducerListener = mProducerListener;
        cloneContainer.mLastOrder = mLastOrder;
        cloneContainer.mIsClone = true;
    }
  
    void Container::Clear(bool doDelete) {
        if(doDelete) {
            vector<Property*>::iterator pitr;
            for(pitr=OrderedProperties->begin();pitr!=OrderedProperties->end();++pitr) {
                delete *pitr;
            }
        }
        OrderedProperties->clear();
        HashedProperties->clear();
        HashMap::iterator itr;
        for(itr=mCategorisedProperties->begin();itr!=mCategorisedProperties->end();++itr) {
            itr->second->clear();
        }
        mCategoryCache->clear();
        mLastOrder = 0;
    }

    void Container::Clear(const ClassTypeInfo& categoryType,bool doDelete) {
        vector<Property*>* categoryVector = GetClosestCategory(categoryType);
        if (categoryVector == NULL) {
            return;
        }
        vector<Property*>::reverse_iterator itr;
        int classID = categoryType.ClassID;
        for(itr=categoryVector->rbegin();itr!=categoryVector->rend();itr++) {
            Property* p = *itr;
            if (classID==p->GetClassTypeInfo()->ClassID) {
                p = Remove(*p->Identity);
                if(doDelete && p!=NULL) {
                    delete p;
                }
            }
        }
    }
#pragma endregion

#pragma region // Initialisation methods.
    ClassTypeInfo::HashSet* Container::CreatePropertyTypes(const ClassTypeInfo* first,...) {
        va_list argPtr;
        va_start(argPtr, first);
        ClassTypeInfo::HashSet* hm = new ClassTypeInfo::HashSet();
        while(first!=NULL) {
            hm->insert(first);
            first = (ClassTypeInfo*)(va_arg(argPtr,ClassTypeInfo*));
        }
        return hm;
    }
#pragma endregion

#pragma region // Content methods.
    void Container::RefreshContents() {
        Flags.Refresh = false;
    }

    bool Container::Contains(const Identifier& identifier) const {
        return GetProperty(identifier)!=NULL;
    }

    bool Container::IsMember(const Property& prop) const {
        return GetProperty(*prop.Identity) == &prop;
    }

    int Container::CategoryCount(const ClassTypeInfo& categoryType) const {
        vector<Property*>* t = GetCategory(categoryType);
        return t == NULL ? 0 : (int)(t->size());
    }

    vector<Property*>* Container::GetClosestCategory(const ClassTypeInfo& type) {
        HashMap::iterator itr = mCategoryCache->find(&type);
        if(itr!=mCategoryCache->end()) {
            return itr->second;
        }
        itr = mCategorisedProperties->find(&type);
        vector<Property*>* closestCategory;
        if(itr!=mCategorisedProperties->end()) {
            closestCategory = itr->second;
        } else {
            int minInheritanceCount = -1;
            HashMap::const_iterator itr;
            closestCategory = NULL;
            for(itr=mCategorisedProperties->begin();itr!=mCategorisedProperties->end();++itr) {
                const ClassTypeInfo* p = itr->first;
                int inheritanceCount = ClassTypeInfo::GetInheritanceDifference(*p,type);
                if(inheritanceCount>=0) {
                    if (minInheritanceCount < 0 || inheritanceCount < minInheritanceCount) {
                        closestCategory = itr->second;
                        minInheritanceCount = inheritanceCount;
                    }
                }
            }
            if (closestCategory == NULL) {
                return NULL;
            }
        }
        (*mCategoryCache)[&type] = closestCategory;
        return closestCategory;
    }

    int Container::BinarySearch(int order) const {
        vector<Property*>::iterator pVector = OrderedProperties->begin();
        int len = (int)(OrderedProperties->size());
        int first = 0;
        int result,half,middle;
        Property *p;
        while (len > 0) {
            half = len >> 1;
            middle = first + half;
            p = pVector[middle];
            result = order - p->Order;
            if (result == 0) {
                return middle;
            } else if (result > 0) {
                first = middle;
                ++first;
                len = len - half - 1;
            } else {
                len = half;
            }
        }
        return ~first;
    }

    int Container::BinarySearch(int order, const vector<Property*>& searchVector) const {
        const vector<Property*>::const_iterator pVector = searchVector.begin();
        int len = (int)(searchVector.size());
        int first = 0;
        int result,half,middle;
        Property *p;
        while (len > 0) {
            half = len >> 1;
            middle = first + half;
            p = pVector[middle];
            result = order - p->Order;
            if (result == 0) {
                return middle;
            } else if (result > 0) {
                first = middle;
                ++first;
                len = len - half - 1;
            } else {
                len = half;
            }
        }
        return ~first;
    }

    Property* Container::GetProperty(const Identifier& identifier) const {
        HashTable::iterator itr = HashedProperties->find(&identifier);
        return (itr==HashedProperties->end()) ? NULL : itr->second;
    }

    Property* Container::GetProperty(int order) const {
        vector<Property*>::iterator pVector = OrderedProperties->begin();
        int len = (int)(OrderedProperties->size());
        int first = 0;
        int result,half,middle;
        Property *p;
        while (len > 0) {
            half = len >> 1;
            middle = first + half;
            p = pVector[middle];
            result = order - p->Order;
            if (result == 0) {
                return p;
            } else if (result > 0) {
                first = middle;
                ++first;
                len = len - half - 1;
            } else {
                len = half;
            }
        }
        return NULL;
    }

    void Container::SetGeometry(Geometry& geometry) {
        ContainerGeometry = &geometry;
        ContainerGeometry->GeometryContainer = this;
    }

    bool Container::Add(Property& prop) {
        // Check property type is a member of the allowed categories.
        vector<Property*>* categoryVector = GetClosestCategory(*(prop.GetClassTypeInfo()));
        if (categoryVector == NULL) {
            return false;
        }
        // Insert into hashed and ordered lists.
        HashTable::iterator itr = HashedProperties->find(prop.Identity);
        if(itr!=HashedProperties->end()) {
            // DEBUG: Overwriting existing: remove first with delete?
            Remove(*itr->second->Identity);
        }
        (*HashedProperties)[prop.Identity] = &prop;
        OrderedProperties->push_back(&prop);
        categoryVector->push_back(&prop);
        prop.Order = mLastOrder++;
        if(ContainerModel->IsListeningToContainers) {
            ContainerModel->AddedProperty(*this, prop);
        }
        return true;
    }

    bool Container::Add(Container& container) {
        if (container.Identity->Position==NULL) {
            // Set default position of property based on parent container shape.
            container.Identity->Position = ContainerGeometry->GetPoint(CoordinateOperations::New,NULL);
        }
        return Add((Property&)container);
    }

    bool Container::Add(Producer& producer) {
        if (Add((Property&)producer)) {
            if (mProducerListener == NULL) {
                mProducerListener = new ProducerListener(*this);
            }
            return true;
        }
        return false;
    }

    bool Container::Add(Restrictor& restrictor) {
        IsUnrestricted = false;
        return Add((Property&)restrictor);
    }

    Property* Container::Remove(const Identifier& ident) {
        HashTable::iterator itr = HashedProperties->find(&ident);
        if(itr!=HashedProperties->end()) {
            Property* p = itr->second;
            HashedProperties->erase(&ident);
            int index = BinarySearch(p->Order);
            if (index>=0) {
                OrderedProperties->erase(OrderedProperties->begin()+index);
            }    
            vector<Property*>* categoryVector = GetClosestCategory(*(p->GetClassTypeInfo()));
            if (categoryVector != NULL) {
                index = BinarySearch(p->Order,*categoryVector);
                if (index>=0) {
                    categoryVector->erase(categoryVector->begin()+index);
                }
            }
            return p;
        }
        return NULL;
    }
#pragma endregion

#pragma region // Access methods.
    Property* Container::PropertyFactory(const Identifier& identifier, const Property* requester) {
        // DEBUG: a default implementation might look for properties with matching identifier
        // and use type of property to create new property.
        return NULL;
    }

    Property* Container::DirectToProperty(const Identifier* identifier, const Property* source, 
            const Property* requester, PropertyModesEnum mode) {
        Property* p = NULL;
        Identifier* directorName;
        // Make sure we have a valid Director name and strip off TypeName and Position.
        if (identifier==NULL || identifier->Name->empty()) {
            // DEBUG: TO DO: Allow container to determine other names for input and output.
            if ((mode & PropertyModes::IsIO) != 0) {
                directorName = new Identifier(*new string(((mode & PropertyModes::Input) != 0) 
                    ? *Director::InputDirectorName : *Director::OutputDirectorName));
            } else {
                directorName = new Identifier(*new string(*Director::DefaultDirectorName));
            }
        } else {
            directorName = new Identifier(*new string(*(identifier->Name)));
        }
        // If the named director exists, let it find the property.
        Property* q = GetProperty(*directorName, requester, mode);
        delete directorName;
        if(q!=NULL && q->Flags.IsDirector) {
            p = ((Director*)q)->Route(identifier, source, requester, mode);
        }
        // Director routing failed, so try fetching a simple property.
        if (p == NULL) {
            Identifier* ident;
            if ((mode & PropertyModes::IsIO) != 0) {
                ident = new Identifier(*new string(((mode & PropertyModes::Input) != 0) 
                    ? *Director::InputName : *Director::OutputName));
            } else {
                ident = new Identifier(*new string(*Director::DefaultName));
            }
            p = GetProperty(*ident, requester, mode);
            delete ident;
        }
        // If a container property was found, recurse until a non-container property is found.
        if (p != NULL) {
            q = p->GetValue();
            if (q->Flags.IsContainer) {
                return ((Container*)q)->DirectToProperty(identifier, source, requester, mode);
            }
        }
        return p;
    }

    Property* Container::ProducerCreate(const Identifier& identifier, const Property* requester) {
        if (mProducerListener != NULL) {
            Property* p = mProducerListener->Create(identifier, requester);
            if (p != NULL) {
                return p;
            }
        }
        // Call PropertyFactory.
        return PropertyFactory(identifier, requester);
    }

    void Container::ProducerPopulate(const IdentifierRegex& regexIdentifier, const Property* requester) {
        if (mProducerListener != NULL) {
            mProducerListener->Populate(regexIdentifier, requester);
        } else {
            // Call default Producer.
            Producer* producer = NULL;
            Identifier ident("DefaultProducer",NULL,NULL);
            Property* p = Container::FindProperty(Parent, ident, requester);
            if (p != NULL) {
                p = p->GetValue(); 
                if(p->Flags.IsProducer) {
                    producer = (Producer*)p;
                }
            }
            if (producer == NULL) {
                // DEBUG: TO DO: work out a generic producer for populating.
                //producer = Producer.DefaultProducer;
                return;
            }
            producer->Populate(*this, regexIdentifier, requester);
        }
    }

    Property* Container::GetProperty(const Identifier& identifier, const Property* requester, PropertyModesEnum mode) {
        HashTable::iterator itr = HashedProperties->find(&identifier);
        Property* p = (itr==HashedProperties->end()) ? NULL : itr->second;
        if (p == NULL && ((mode & PropertyModes::Producing) != 0)) {
            p = ProducerCreate(identifier, requester);
        }
        if (p != NULL && requester != NULL) {
            if (!p->InScope(requester, false)) {
                return NULL;
            }
        }
        return p;
    }

    IdentifierEnumerator* Container::GetProperties(const IdentifierRegex& regexIdentifier, const ClassTypeInfo& categoryType, 
            bool includeWhenMatches, const Property* requester, PropertyModesEnum mode) {
        if ((mode & PropertyModes::Producing) != 0) {
            ProducerPopulate(regexIdentifier, requester);
        }
        // DEBUG: Need a slice & dice call here for potential regex point.
        vector<Property*>* properties = GetCategory(categoryType);
        if(properties==NULL) return NULL;
        return new IdentifierEnumerator(*properties, *new IdentifierRegex(regexIdentifier), includeWhenMatches, requester);
    }

    int* Container::GetOrderID(const Container* parent, const Property& prop) {
        int* orderID;
        if (parent != NULL) {
            const int* tmpID = parent->mOrderID;
            int len = tmpID[0];
            orderID = new int[1 + ++len];
            for(int index=1;index<len;index++) orderID[index] = tmpID[index];
            orderID[len] = prop.Order;
            orderID[0] = len;
        }
        else {
            orderID = new int[2];
            orderID[0] = 1;
            orderID[1] = 0;
        }
        return orderID;
    }

    int Container::CompareToOrder(const int* a, const int* b) {
        int aLen = a[0];
        int bLen = b[0];
        for (int index = 1; index <= aLen; index++) {
            if (index > bLen) {
                return 1;
            }
            int av = a[index];
            int bv = b[index];
            if (av < bv) {
                return -1;
            }
            if (av > bv) {
                return 1;
            }
        }
        if (bLen > aLen) {
            return -1;
        }
        return 0;
    }

    int Container::OrderDifferIndex(const int* a, const int* b) {
        int minLen = a[0];
        int maxLen = b[0];
        if (minLen > maxLen) {
            minLen = maxLen;
            maxLen = a[0];
        }
        for (int index = 1; index <= minLen; index++) {
            if (a[index] != b[index]) {
                return index;
            }
        }
        return maxLen;
    }

    Property* Container::GetProperty(const Container* container, const int* orderID) {
        if (container == NULL) {
            return NULL;
        }
        int len = orderID[0]-1;
        Property* p = NULL;
        for (int index = 1; index <= len; ++index) {
            p = container->GetProperty(orderID[index]);
            if(p==NULL) {
                break;
            }
            if (index < len) {
                p = p->GetValue(); // DEBUG: Should it go through GetValue()?
                if(p->Flags.IsContainer) {
                    container = (Container*)p;
                } else {
                    return NULL;
                }
            }
        }
        return p;
    }
  
    Property* Container::FindProperty(Container* searchContainer, const Identifier& identifier, const Property* requester) {
        Property* prop = NULL;
        while (searchContainer != NULL) {
            prop = searchContainer->GetProperty(identifier,requester,PropertyModes::Traversing);
            if(prop!=NULL) {
                break;
            }
            searchContainer = searchContainer->Parent;
        }
        return prop;
    }

    Property* Container::FindProperty(Container* searchContainer, const Identifier& identifier, const Property* requester,
            bool includeWhenMatches, bool searchUp, Container*& parentContainer) {
        vector<Property*>::iterator itr;
        vector<Property*>* searchVector;
        while (searchContainer != NULL) {
            // Check if property in current container.
            Property* prop = searchContainer->GetProperty(identifier,requester,PropertyModes::Traversing);
            if(prop!=NULL) {
                if(includeWhenMatches) {
                    parentContainer = searchContainer;
                    return prop;
                }
                searchVector = searchContainer->OrderedProperties;
                for(itr=searchVector->begin();itr!=searchVector->end();++itr) {
                    prop = *itr;
                    if (identifier.CompareTo(*prop->Identity)!=0) {
                        parentContainer = searchContainer;
                        return prop;
                    }
                }
            }
            if(searchUp) {
                searchContainer = searchContainer->Parent;
            } else {
                // Check child containers.
                searchVector = searchContainer->OrderedProperties;
                for(itr=searchVector->begin();itr!=searchVector->end();++itr) {
                    prop = *itr;
                    Property *p = prop->GetValue();
                    if (p->Flags.IsContainer) {
                        prop = FindProperty((Container*)p, identifier, requester, includeWhenMatches, searchUp, parentContainer);
                        if (prop != NULL) {
                            return prop;
                        }
                    }
                }
                break;
            }
        }
        parentContainer = NULL;
        return NULL;
    }

    Property* Container::FindProperty(Container* searchContainer, const IdentifierRegex& regex, const Property* requester,
            bool includeWhenMatches, bool searchUp, Container*& parentContainer) {
        while (searchContainer != NULL) {
            vector<Property*>::iterator itr;
            vector<Property*>& searchVector = *(searchContainer->OrderedProperties);
            for(itr=searchVector.begin();itr!=searchVector.end();++itr) {
                Property* prop = *itr;
                if (regex.Match(*prop->Identity)) {
                    if (includeWhenMatches && prop->InScope(requester, false)) {
                        parentContainer = searchContainer;
                        return prop;
                    }
                } else {
                    if (!includeWhenMatches && prop->InScope(requester, false)) {
                        parentContainer = searchContainer;
                        return prop;
                    }
                }
                if (!searchUp) {
                    Property *p = prop->GetValue();
                    if (p->Flags.IsContainer) {
                        prop = FindProperty((Container*)p, regex, requester, includeWhenMatches, searchUp, parentContainer);
                        if (parentContainer != NULL) {
                            return parentContainer;
                        }
                    }
                }
            }
            searchContainer = (searchUp ? searchContainer->Parent : NULL);
        }
        parentContainer = NULL;
        return NULL;
    }
#pragma endregion

#pragma region /// Model updating methods.
    void Container::Update() {
    }
    void Container::BirthDeath() {
    }
    void Container::Superposition() {
    }
#pragma endregion
}
