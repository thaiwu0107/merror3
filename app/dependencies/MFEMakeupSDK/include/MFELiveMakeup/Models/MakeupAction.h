//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include <memory>

#include "MakeupProduct.h"
#include "LashModel.h"
#include "Masks/MakeupMask.h"

#include "MFELiveCommon.h"

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup Models
     *  @{
     */
    
    class MakeupAction {
    public:
        /**
         * A list of MakeupAction classes. This is used to avoid RTTI.
         */
        enum class ClassType {
            MakeupAction,
            MakeupActionDrawMakeup,
            MakeupActionErase,
            MakeupActionDrawLash,
            MakeupActionDrawOverlay
        };
        
        virtual ~MakeupAction() { };

        /**
         * This is called when new tracking data available. Some actions may use this
         * data (e.g. updating masks).
         */
        virtual void prepareWithTrackingData(const TrackingData &data) { };

        virtual bool equals(const MakeupAction &other) const;

        virtual std::shared_ptr<MakeupAction> clone() const;
        
        /**
         * A static method that returns the class type. This must be implemented in all subclasses.
         */
        static ClassType staticType() { return ClassType::MakeupAction; };
        
        /**
         * A virtual method that returns the class type. This must be implemented in all subclasses.
         */
        virtual ClassType getClassType() const { return staticType(); };
        
        /**
         * Attempts to cast the given MakeupAction pointer to some subclass T. If the cast is invalid, then
         * this will return nullptr, otherwise this will return the casted pointer.
         */
        template<typename T> static std::shared_ptr<T> castTo(std::shared_ptr<MakeupAction> ptr) {
            static_assert(std::is_base_of<MakeupAction, T>::value, "T must be a subclass of MakeupAction");
            assert(ptr);
            
            if (ptr->getClassType() == T::staticType()) {
                return std::static_pointer_cast<T>(ptr);
            }
            else {
                return nullptr;
            }
        };
    };

    typedef std::shared_ptr<MakeupAction> MFEMakeupActionPtr;

    /**
     * For drawing makeup.
     * Accepted by the following filter types:
     * General, BrowRecolour, Foundation, Lipstick
     */
    class MakeupActionDrawMakeup : public MakeupAction {
    public:
        virtual ~MakeupActionDrawMakeup() { };

        /**
         * The mask for the action.
         */
        std::shared_ptr<MakeupMask> mask;

        /**
         * The material properties of the makeup product.
         */
        MakeupProduct product;

        void prepareWithTrackingData(const TrackingData &data) override {
            if (mask) {
                mask->updateWithTrackingData(data);
            }
        };

        std::shared_ptr<MakeupAction> clone() const override;

        bool equals(const MakeupAction &other) const override;
        
        static ClassType staticType() { return ClassType::MakeupActionDrawMakeup; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupActionDrawMakeup> MakeupActionDrawMakeupPtr;

    /**
     * For erasing what has been  drawn.
     * Accepted by the following filter types:
     * General, BrowRecolour, Foundation, Lipstick, Overlay
     */
    class MakeupActionErase : public MakeupAction {
    public:
        virtual ~MakeupActionErase() { };

        /**
         * The mask for the action.
         */
        std::shared_ptr<MakeupMask> mask;

        /**
         * Amount to erase, ranging from 0.0 to 1.0.
         */
        float eraseAmount;

        void prepareWithTrackingData(const TrackingData &data) override {
            if (mask) {
                mask->updateWithTrackingData(data);
            }
        };

        MakeupActionErase() : eraseAmount(0) { };

        std::shared_ptr<MakeupAction> clone() const override;

        bool equals(const MakeupAction &other) const override;
        
        static ClassType staticType() { return ClassType::MakeupActionErase; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupActionErase> MakeupActionErasePtr;

    /**
     * For drawing lashes and mascara.
     * Accepted by the following filter types:
     * Lashes
     */
    class MakeupActionDrawLash : public MakeupAction {
    public:
        virtual ~MakeupActionDrawLash() { };

        /**
         * The lash model.
         */
        LashModel lashModel;
        bool isLeftSide;

        void prepareWithTrackingData(const TrackingData &data) override { };

        MakeupActionDrawLash() : isLeftSide(false) { };

        std::shared_ptr<MakeupAction> clone() const override;

        bool equals(const MakeupAction &other) const override;
        
        static ClassType staticType() { return ClassType::MakeupActionDrawLash; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupActionDrawLash> MakeupActionDrawLashPtr;

    /**
     * For drawing overlays for tutorials. It can draw a color overlay or
     * an image overlay.
     * Accepted by the following filter types:
     * Overlay
     */
    class MakeupActionDrawOverlay : public MakeupAction {
    public:
        virtual ~MakeupActionDrawOverlay() { };

        /**
         * The mask for the action.
         */
        std::shared_ptr<MakeupMask> mask;

        /**
         * The color overlay, if using a color overlay.
         * If overlayImagePath is not specified, then overlayColor is used.
         */
        cv::Scalar overlayColor;

        /**
         * The overlay image path, if using an image overlay.
         * Use empty string to if no overlay image is used.
         * If this is specified, the value of overlayColor is ignored.
         */
        std::string overlayImagePath;

        /**
         * Whether the overlay image is horizontally flipped
         */
        bool isOverlayImageHorizontallyFlipped;

        void prepareWithTrackingData(const TrackingData &data) override {
            if (mask) {
                mask->updateWithTrackingData(data);
            }
        };

        MakeupActionDrawOverlay() : isOverlayImageHorizontallyFlipped(false) { };

        std::shared_ptr<MakeupAction> clone() const override;

        bool equals(const MakeupAction &other) const override;
        
        static ClassType staticType() { return ClassType::MakeupActionDrawOverlay; };
        virtual ClassType getClassType() const override { return staticType(); };
    };

    typedef std::shared_ptr<MakeupActionDrawOverlay> MakeupActionDrawOverlayPtr;

    // Equality operators
    bool operator==(const MakeupAction &lhs, const MakeupAction &rhs);

    bool operator!=(const MakeupAction &lhs, const MakeupAction &rhs);

    /** @}*/
}

#pragma GCC visibility pop
