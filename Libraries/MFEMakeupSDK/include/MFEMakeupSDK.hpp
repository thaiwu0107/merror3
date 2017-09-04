//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#if __ANDROID__
#include "jni.h"
#endif

/** \addtogroup MFEMakeupSDK
 * Main library for applying makeup
 */

/** \addtogroup Models
 * Classes that define what makeup and how the makeup are applied.
 * Main classes are MFE::MakeupLook, MFE::MakeupProcedure, MFE::MakeupAction, MFE::MakeupProduct, MFE::MakeupMask
 * \ingroup MFEMakeupSDK
 */

/** \addtogroup MFELiveCommon
 * Utility, helper and extra classes for easier use of MFEMakeupSDK.
 * \ingroup MFEMakeupSDK
 */

/** \addtogroup MFELiveCommon_Models
 * Model classes
 * \ingroup MFELiveCommon
 */

#pragma GCC visibility push(default)

namespace MFE {

    /** \addtogroup MFEMakeupSDK
     *  @{
     */

#if __ANDROID__
    /**
     * Important!!
     * Must call MFE::MFEMakeupSDK_init(JNIEnv*, jobject) before any MFEMakeupSDK components are initialized.
     *
     * The jni parameters must be non-null and the jobject parameter must be a
     * android.content.Context java class object of the application this SDK is running on.
     */
    void MFEMakeupSDK_init(JNIEnv *env, jobject context);
#else
    /**
     * Important!!
     * Must call MFE::MFEMakeupSDK_init() before any MFEMakeupSDK components are initialized.
     */
    void MFEMakeupSDK_init();
#endif

    /** @}*/

}

#pragma GCC visibility pop
