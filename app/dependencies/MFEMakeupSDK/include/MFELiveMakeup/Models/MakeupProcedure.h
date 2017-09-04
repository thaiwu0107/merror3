//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include <vector>
#include <memory>

#include "MakeupProduct.h"
#include "Masks/MakeupMask.h"
#include "MakeupAction.h"

#pragma GCC visibility push(default)

namespace MFE {

    /** \addtogroup Models
     *  @{
     */

    /**
     * A makeup procedure defines a list of actions required to draw a particular style and type of makeup and each
     * subclass of a procedure corresponds with a particular type of filter. Different types may accept different actions.
     * For example, a draw lash procedure might only accept draw lash actions. The actions are rejected by quitting
     * the application with a assertion failure.
     */
    class MakeupProcedure {
    public:
        /**
         * A list of MakeupProcedure classes. This is used to avoid RTTI.
         */
        enum class ClassType {
            MakeupProcedure,
            MakeupProcedureGeneral,
            MakeupProcedureBrowRecolour,
            MakeupProcedureFoundation,
            MakeupProcedureLashes,
            MakeupProcedureLipstick,
            MakeupProcedureOverlay
        };
        
        virtual ~MakeupProcedure();

        /**
         * An optional name for the procedure, for debugging purposes.
         */
        std::string name;

        /**
         * A list of actions for this procedure. The actions are applied in the same order as they
         * appear in the vector. For example, the first item in the vector is the first action that
         * will be performed.
         */
        std::vector<std::shared_ptr<MakeupAction>> actions;

        /**
         * Returns a name for the current procedure type, which is
         * unique to the particular subclass.
         */
        virtual std::string getProcedureTypeName() const;

        /**
         * Performs a deep copy.
         */
        virtual std::shared_ptr<MakeupProcedure> clone() const;

        /**
         * Checks for equality.
         */
        virtual bool equals(const MakeupProcedure &other) const;

        /**
         * A static method that returns the class type. This must be implemented in all subclasses.
         */
        static ClassType staticType() { return ClassType::MakeupProcedure; };
        
        /**
         * A virtual method that returns the class type. This must be implemented in all subclasses.
         */
        virtual ClassType getClassType() const { return staticType(); };
        
        /**
         * Attempts to cast the given MakeupProcedure pointer to some subclass T. If the cast is invalid, then
         * this will return nullptr, otherwise this will return the casted pointer.
         */
        template<typename T> static std::shared_ptr<T> castTo(std::shared_ptr<MakeupProcedure> ptr) {
            static_assert(std::is_base_of<MakeupProcedure, T>::value, "T must be a subclass of MakeupProcedure");
            assert(ptr);
            
            if (ptr->getClassType() == T::staticType()) {
                return std::static_pointer_cast<T>(ptr);
            }
            else {
                return nullptr;
            }
        };
        
    protected:
        void baseCopyTo(MakeupProcedure &other) const;
    };

    typedef std::shared_ptr<MakeupProcedure> MakeupProcedurePtr;


    /**
     * For general application of makeup. Useful for blush, eyeshadow, liner, etc.
     */
    class MakeupProcedureGeneral : public MakeupProcedure {
    public:
        virtual ~MakeupProcedureGeneral();

        std::string getProcedureTypeName() const override;

        std::shared_ptr<MakeupProcedure> clone() const override;
        
        static ClassType staticType() { return ClassType::MakeupProcedureGeneral; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupProcedureGeneral> MakeupProcedureGeneralPtr;


    /**
     * For brow recolouring.
     */
    class MakeupProcedureBrowRecolour : public MakeupProcedure {
    public:
        virtual ~MakeupProcedureBrowRecolour();

        std::string getProcedureTypeName() const override;

        std::shared_ptr<MakeupProcedure> clone() const override;
        
        static ClassType staticType() { return ClassType::MakeupProcedureBrowRecolour; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupProcedureBrowRecolour> MakeupProcedureBrowRecolourPtr;


    /**
     * Similar to general, but also does skin smoothing.
     */
    class MakeupProcedureFoundation : public MakeupProcedure {
    public:
        virtual ~MakeupProcedureFoundation();

        std::string getProcedureTypeName() const override;

        std::shared_ptr<MakeupProcedure> clone() const override;
        
        static ClassType staticType() { return ClassType::MakeupProcedureFoundation; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupProcedureFoundation> MakeupProcedureFoundationPtr;


    /**
     * For drawing false lashes or mascara.
     */
    class MakeupProcedureLashes : public MakeupProcedure {
    public:
        virtual ~MakeupProcedureLashes();

        std::string getProcedureTypeName() const override;

        std::shared_ptr<MakeupProcedure> clone() const override;
        
        static ClassType staticType() { return ClassType::MakeupProcedureLashes; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupProcedureLashes> MakeupProcedureLashesPtr;


    /**
     * Similar to general, but provides more detailed shine and also a wetness effect.
     */
    class MakeupProcedureLipstick : public MakeupProcedure {
    public:
        virtual ~MakeupProcedureLipstick();

        bool reduceShineOnEdges;

        MakeupProcedureLipstick() : reduceShineOnEdges(true) { };

        std::string getProcedureTypeName() const override;

        std::shared_ptr<MakeupProcedure> clone() const override;

        bool equals(const MakeupProcedure &other) const override;
        
        static ClassType staticType() { return ClassType::MakeupProcedureLipstick; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupProcedureLipstick> MakeupProcedureLipstickPtr;


    /**
     * For drawing solid or textured overlays. Useful for tutorials.
     */
    class MakeupProcedureOverlay : public MakeupProcedure {
    public:
        virtual ~MakeupProcedureOverlay();

        std::string getProcedureTypeName() const override;

        std::shared_ptr<MakeupProcedure> clone() const override;
        
        static ClassType staticType() { return ClassType::MakeupProcedureOverlay; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupProcedureOverlay> MakeupProcedureOverlayPtr;


    // Equality operators
    bool operator==(const MakeupProcedure &lhs, const MakeupProcedure &rhs);

    bool operator!=(const MakeupProcedure &lhs, const MakeupProcedure &rhs);

    /** @}*/
}

#pragma GCC visibility pop
