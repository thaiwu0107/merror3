//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include "MakeupProcedure.h"

#pragma GCC visibility push(default)

namespace MFE {

    /** \addtogroup Models
     *  @{
     */

    /**
     * A look is a collection of procedures.
     */
    class MakeupLook {
    public:
        MakeupLook() { };

        MakeupLook(const MakeupLook &look);

        virtual ~MakeupLook();

        MakeupLook &operator=(const MakeupLook &other);

        std::vector<std::shared_ptr<MakeupProcedure>> procedures;

        //! Whether the look is empty. A look is considered empty if it contains no procedures.
        bool isEmpty() const { return procedures.empty(); }
        
        // Equality operators
        bool operator==(const MakeupLook &rhs) const;

        bool operator!=(const MakeupLook &rhs) const;
    };

    /** @}*/
}

#pragma GCC visibility pop
