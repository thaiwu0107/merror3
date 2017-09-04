package com.merrors.merror.app.utils;

import android.content.Context;
import android.util.Log;
import org.apache.commons.io.IOUtils;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


/**
 * Utility for copying asset files to device storage
 */
public class AssetFilesCopier {

    private static final String TAG = "AssetFilesCopier";

    private static boolean isSameFileStructure(Context context, String assetDirPath, File exportedDir){
        if (assetDirPath.startsWith("asset://"))
            assetDirPath = assetDirPath.substring("asset://".length());

        boolean same = true;
        try {
            // Check folder
            String[] assetContents = context.getAssets().list(assetDirPath);
            String[] exportedContents = exportedDir.list();

            if (assetContents == null){
                throw new RuntimeException("Could not find asset folder: " + assetDirPath);
            }

            if (exportedContents == null){
                same = false;
            }

            if (same){
                if (assetContents.length == exportedContents.length){
                    for (int i = 0; i < assetContents.length; ++i){
                        if (assetContents[i].equalsIgnoreCase(exportedContents[i]) == false){
                            same = false;
                        }
                    }
                }
                else {
                    same = false;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return same;
    }

    /**
     * Check to see if assetPath is a directory with at lease one file (can be sub directory) in it.
     *
     * @return true if it is an asset directory AND has at lease one file in it.
     */
    public static boolean isAssetDirectoryWithContents(Context context, String assetPath) {
        if (assetPath == null || assetPath.length() == 0)
            return false;

        if (assetPath.startsWith("asset://"))
            assetPath = assetPath.substring("asset://".length());

        String[] contents = null;
        try {
            contents = context.getAssets().list(assetPath);
        } catch (IOException e) {
            e.printStackTrace();
        }

        return !(contents == null || contents.length == 0);
    }

    /**
     * @param fileName
     * @return "aoeu.a.b" becomes "a.b", "aoeu.ext" returns "ext"
     */
    public static String getExtension(String fileName) {
        int slash = fileName.lastIndexOf("/") + 1;
        if (slash < 0)
            slash = 0;

        // hidden file so we skill all starting dots
        while (fileName.substring(slash, slash + 1).equals(".")) {
            slash++;
        }
        int firstDot = fileName.indexOf(".", slash);
        if (firstDot < 0)
            return null;
        if (firstDot >= fileName.length() - 1)
            return null;
        return fileName.substring(firstDot + 1);
    }

    /**
     * Copy asset file to on-device file
     *
     * @return true on success
     */
    public static boolean copyAssetToFile(Context context, String assetPath, File dstFile) {
        if (assetPath == null || assetPath.length() == 0)
            return false;

        if (assetPath.startsWith("asset://"))
            assetPath = assetPath.substring("asset://".length());

        boolean success = false;

        InputStream in = null;
        OutputStream out = null;
        try {
            if (dstFile.exists() && !dstFile.isFile())
                throw new IOException(dstFile.getPath() + " must be a file.");

            in = context.getAssets().open(assetPath);
            out = new FileOutputStream(dstFile);
            IOUtils.copy(in, out);
            success = true;
        } catch (IOException | NullPointerException e) {
            e.printStackTrace();
        } finally {
            IOUtils.closeQuietly(in);
            IOUtils.closeQuietly(out);
        }

        // Delete upon failure
        if (!success) {
            dstFile.delete();
        }

        return success;
    }

    /**
     * Copy asset directory ('assetPath') to 'dstDir' directory recursively. Will not delete 'dstDir' upon failure. </br>
     * NOTE: This function will treat asset file with no extension as empty folder.
     *
     * @return true on success
     */
    public static boolean copyAssetDirToDir(Context context, String assetPath, File dstDir) {
        if (assetPath == null || assetPath.length() == 0 || dstDir == null)
            return false;

        if (!dstDir.exists())
            dstDir.mkdir();

        if (assetPath.startsWith("asset://"))
            assetPath = assetPath.substring("asset://".length());

        String[] contents = null;
        try {
            contents = context.getAssets().list(assetPath);
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (contents != null && contents.length > 0 && dstDir.isDirectory()) {
            for (String filename : contents) {

                // Listen for interrupt
                boolean interrupted = Thread.currentThread().isInterrupted();
                if (interrupted)
                    return false;

                String assetFilePath = assetPath + "/" + filename;
                if (isAssetDirectoryWithContents(context, assetFilePath)) {
                    // Recursion!
                    File copyDir = new File(dstDir, filename);
                    boolean success = copyAssetDirToDir(context, assetFilePath, copyDir);
                    if (!success)
                        return false;
                } else {
                    String ext = getExtension(filename);
                    if (ext == null) {
                        Log.d(TAG, "No extension found. Skipping asset: " + assetFilePath);
                    } else {
                        File copyFile = new File(dstDir, filename);
                        boolean success = copyAssetToFile(context, assetFilePath, copyFile);
                        if (!success)
                            return false;
                    }
                }
            }

            return true;
        }

        return false;
    }

    public static String copyData(Context context, String assetDirPath, String saveDirPath) {
        File defaultSaveDirectory = context.getFilesDir();
        File location = new File(defaultSaveDirectory, saveDirPath);
        if(isSameFileStructure(context, "asset://"+assetDirPath, location)) {
    		Log.d(TAG, "asset path files not copied to dir:"+location.getAbsolutePath());
            return location.getAbsolutePath();
        }
        Log.d(TAG, "AFC copy assets to dir is " + copyAssetDirToDir(context, assetDirPath, location) + ": "  + location.getAbsolutePath());
        return location.getAbsolutePath();
    }
}