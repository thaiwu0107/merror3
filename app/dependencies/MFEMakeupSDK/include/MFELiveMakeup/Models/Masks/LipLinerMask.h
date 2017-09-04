//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include "MakeupMask.h"

#pragma GCC visibility push(default)

namespace MFE {
	class LipLinerMask : public MakeupMask {
	public:
		LipLinerMask();

		/*
		Parameters
		*/

		/**
		How thick the lip should be. Default value is 2.25.
		*/
		float thicknessFactor;

		/**
		How much to blur the outer part of the mask. Default value is 1.5.
		*/
		float blurFactor;

		void updateWithTrackingData(const TrackingData &data) override;
		bool isStaticMask() const override;
		bool equals(const MakeupMask& other) const override;
		std::shared_ptr<MakeupMask> clone() const override;
	};

	typedef std::shared_ptr<LipLinerMask> LipLinerMaskPtr;
}

#pragma GCC visibility pop
