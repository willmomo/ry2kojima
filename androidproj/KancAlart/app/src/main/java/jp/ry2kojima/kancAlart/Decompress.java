package jp.ry2kojima.kancAlart;

import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/*
    http://stackoverflow.com/questions/3382996/how-to-unzip-files-programmatically-in-android
*/

/**
 * Created by kojima on 2015/07/28.
 */

/**
 * How to use:
 *
 * String zipFile = Environment.getExternalStorageDirectory() + "/files.zip";
 * String unzipLocation = Environment.getExternalStorageDirectory() + "/unzipped/";
 *
 * Decompress d = new Decompress(zipFile, unzipLocation);
 * d.unzip();
 */
public class Decompress {
    private String _zipFile;
    private String _location;

    public Decompress(String zipFile, String location) {
        _zipFile = zipFile;
        _location = location;

        _dirChecker("");
    }

    public void unzip(String pattern) {
        try  {
            FileInputStream fin = new FileInputStream(_zipFile);
            ZipInputStream zin = new ZipInputStream(fin);
            ZipEntry ze = null;
            while ((ze = zin.getNextEntry()) != null) {

                if(ze.isDirectory()) {
                    _dirChecker(ze.getName());
                } else {
                    //Log.v("Decompress", "pattern = " + pattern);
                    //Log.v("Decompress", "getName = " + ze.getName());
                    //Log.v("Decompress", "match? = " + ze.getName().matches(pattern));
                    if (ze.getName().matches(pattern)) {
                        Log.v("Decompress", "Unzipping " + ze.getName());
                        FileOutputStream fout = new FileOutputStream(_location + ze.getName());
                        byte[] buffer = new byte[1024];
                        int count;
                        while ((count = zin.read(buffer)) != -1) {
                            fout.write(buffer, 0, count);
                        }

                        zin.closeEntry();
                        fout.close();
                    }
                }

            }
            zin.close();
        } catch(Exception e) {
            Log.e("Decompress", "unzip", e);
        }

    }

    private void _dirChecker(String dir) {
        Log.v("Decompress", "_dirChecker(" + dir + ")");
        Log.v("Decompress", "_dirChecker: '" + _location + "'");
        File f = new File(_location + dir);

        Log.v("Decompress f1", String.valueOf(f.isDirectory()));
        if(!f.isDirectory()) {
            f.mkdirs();
        }

        File f2 = new File(_location + dir);
        Log.v("Decompress f2", String.valueOf(f2.isDirectory()));
    }
}
