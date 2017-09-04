package com.merrors.merror.camera.views;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.merrors.merror.app.DCode;
import com.merrors.merror.app.utils.AssetFilesCopier;

/**
 * \addtogroup Java
 *   @{
 */

/**
 Display splash screen and copies the necessary assets to local dir to be used with MFEMakeupSDK
 */
public class SplashActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void onResume() {
        CopyAssetsTask task = new CopyAssetsTask(this);
        task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);

        super.onResume();
    }

    class CopyAssetsTask extends AsyncTask<Void,Void,Void> {

        private final Context mContext;

        public CopyAssetsTask(Context context) {
            mContext = context;
        }

        @Override
        protected Void doInBackground(Void... voids) {
            AssetFilesCopier.copyData(mContext, DCode.AssetsConfig.TrackerAssetDir, DCode.AssetsConfig.TrackerSaveDir);
            AssetFilesCopier.copyData(mContext, DCode.AssetsConfig.MFEBundleAssetDir, DCode.AssetsConfig.MFEBundleSaveDir);
            return null;
        }

        @Override
        protected void onPostExecute(Void p) {
            Intent intent = new Intent(SplashActivity.this, MainActivity.class);
            startActivity(intent);
            finish();
        }
    }
}

/** @}*/
