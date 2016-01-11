#include "PlatoIncludes.h"
// This pulls in all the library headers.
#include "PlatoStandard.h"

#define AddInitializer(className) InitializerCatalogueTable->push_back(new InitializerCatalogueEntry(className::Initializer,className::Finalizer))

namespace Plato {

    vector<InitializerCatalogueEntry*>* InitializerCatalogue::InitializerCatalogueTable = NULL;
    
    InitializerCatalogueEntry::InitializerCatalogueEntry(InitializerDelegate initializer, InitializerDelegate finalizer) {
        Initializer = initializer;
        Finalizer = finalizer;
    }

    void InitializerCatalogue::Initialize() {
        if(InitializerCatalogueTable!=NULL) return;
        InitializerCatalogueTable = new vector<InitializerCatalogueEntry*>();

#pragma region // Class initializer details should be added to the table here in dependency order.
        // General
        AddInitializer(Strings); 
        AddInitializer(Regexes); 
        // Configure
        AddInitializer(ClassCatalogue); 
        AddInitializer(Configuration); 
        AddInitializer(DiagnosticsCatalogue); 
        // Primitives
        AddInitializer(Point); 
        AddInitializer(Property); 
        AddInitializer(Container); 
        AddInitializer(Alias); 
        AddInitializer(Link); 
        AddInitializer(Path); 
        AddInitializer(PathNode); 
        // Dependencies
        AddInitializer(Model); 
        // Geometries
        AddInitializer(Scale); 
        AddInitializer(Size); 
        AddInitializer(Geometry); 
        AddInitializer(CylindricalSpace); 
        AddInitializer(SphericalSpace); 
        // Distributions
        // Operators
        AddInitializer(Distributor); 
        AddInitializer(Connector); 
        AddInitializer(Converter); 
        AddInitializer(GroupContainer); 
        // Relators
        AddInitializer(NextXRelator); 
        // Filters
        AddInitializer(Filter); 
        AddInitializer(GroupFilter); 
        AddInitializer(IdentifierFilter); 
        AddInitializer(PointFilter); 
        AddInitializer(NameFilter); 
        AddInitializer(NeighbourhoodFilter); 
        AddInitializer(TypeNameFilter); 
        AddInitializer(VacantFilter);
        AddInitializer(SearchFilter); 
        AddInitializer(TypeFilter); 
        AddInitializer(PathFilter); 
        // Restrictors
        AddInitializer(Restrictor); 
        AddInitializer(PathRestrictor); 
        AddInitializer(ConverterRestrictor); 
        AddInitializer(LocalRestrictor); 
        // Generators
        // Locators
        // Transducers
        AddInitializer(Transducer); 
        AddInitializer(SearchTransducer); 
        // Translators
        AddInitializer(Translator); 
        AddInitializer(ReverseTranslator); 
        AddInitializer(GenericTranslator); 
        // Producers
        AddInitializer(Constructor); 
        AddInitializer(Producer); 
        AddInitializer(SearchProducer);
        // Directors
        AddInitializer(Director); 
        // Elements
        AddInitializer(Pad); 
        AddInitializer(OutputPad); 
        AddInitializer(InputPad); 
        AddInitializer(Bundle);
        AddInitializer(Disposition); 
        AddInitializer(Flow); 
        AddInitializer(Region); 
        AddInitializer(Phenomenon);
#pragma endregion

        // Now call the class initializers.
        vector<InitializerCatalogueEntry*>::iterator itr;
        for(itr=InitializerCatalogueTable->begin();itr!=InitializerCatalogueTable->end();++itr) {
            (*((*itr)->Initializer))();
        }
    }

    void InitializerCatalogue::Finalize() {
        if(InitializerCatalogueTable!=NULL) {
            vector<InitializerCatalogueEntry*>::reverse_iterator itr;
            for(itr=InitializerCatalogueTable->rbegin();itr!=InitializerCatalogueTable->rend();++itr) {
                (*((*itr)->Finalizer))();
                delete *itr;
            }
            delete InitializerCatalogueTable;
            InitializerCatalogueTable = NULL;
        }
    }
}
