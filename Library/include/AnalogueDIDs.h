#pragma once

namespace Plato { 

    /// <summary>
    /// An input pad for an And did.
    /// </summary>
    class AndInputPad : public InputPad {
    public:
        /// <summary>
        /// Standard constructor
        /// </summary>
        /// <param name="creator">The element that created this pad.</param>
        /// <param name="parent">The hierarchical parent of this pad.</param>
        /// <param name="identity">The name to assign to the new pad.</param>
        /// <param name="scope">The visibility of the pad.</param>
        AndInputPad(Container* creator, Container& parent, Identifier& identity, PropertyScopesEnum scope);
        // float mWeight;
    };

    /// <summary>
    /// An output pad for an And did.
    /// </summary>
    class AndOutputPad : public OutputPad {
    public:
        /// <summary>
        /// Standard constructor.
        /// </summary>
        /// <param name="creator">The element that created this pad.</param>
        /// <param name="parent">The hierarchical parent of this pad.</param>
        /// <param name="identity">The name to assign to the new pad.</param>
        /// <param name="scope">The visibility of the pad.</param>
        AndOutputPad(Container* creator, Container& parent, Identifier& identity, PropertyScopesEnum scope);
        // float mWeight;
    };

    /// <summary>
    /// A bundle of And pads.
    /// </summary>
    class AndBundle : public Bundle {
    public:
        /// <summary>
        /// Constructs a new Bundle.
        /// </summary>
        /// <param name="creator">The element that created this element.</param>
        /// <param name="parent">The hierarchical parent of this element.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        /// <param name="isInput">If true, this is an input bundle, otherwise it's an output bundle.</param>
        AndBundle(Container* creator, Container& parent,
            Identifier& elementName = Identifier::Generate("AndBundle"), 
            PropertyScopesEnum scope = PropertyScopes::Public, 
            bool isInput=true);

    protected:
        /// <summary>
        /// Creates a new connection pad with the specified name.
        /// </summary>
        /// <param name="identifier">Identifier of pad to create.
        /// This will typically include a Name and Position, although the Type may not be set.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to fetch, or NULL if scope checking not required.</param>
        /// <returns>Returns the requested Pad property, or NULL if it could not be created.</returns>
        virtual Property* PropertyFactory(Identifier& identifier, const Property* requester);
    };

    /// <summary>
    /// Causally ANDs it's inputs together.
    /// </summary>
    class AndDID : public Disposition {
    public:
        /// <summary>
        /// Create a new AndDID.
        /// </summary>
        /// <param name="creator">The element that created this element.</param>
        /// <param name="parent">The hierarchical parent of this element.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        AndDID(Container* creator = NULL, Container* parent = NULL,
            Identifier& elementName = Identifier::Generate("AndDID"), 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        ///
        /// </summary>
        virtual void Update();

        /// <summary>
        ///
        /// </summary>
        /// <param name="name"></param>
        /// <param name="isInput"></param>
        /// <returns></returns>
        virtual bool AddBundle(Identifier& name, bool isInput);
    };

    /// <summary>
    /// Causally ORs it's inputs together.
    /// </summary>
    class OrDID : public Disposition {
    public:
        /// <summary>
        /// Create a new OrDID.
        /// </summary>
        /// <param name="creator">The element that created this element.</param>
        /// <param name="parent">The hierarchical parent of this element.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        OrDID(Container* creator = NULL, Container* parent = NULL,
            Identifier& elementName = Identifier::Generate("OrDID"), 
            PropertyScopesEnum scope = PropertyScopes::Public);
        /// <summary>
        ///
        /// </summary>
        virtual void Update();
    };
}
