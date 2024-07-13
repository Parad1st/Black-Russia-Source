package com.blackrussia.launcher.other;

import android.content.Intent;
import android.content.Context;

import java.text.DecimalFormat;
import java.io.File;

public class Utils {
	public static final long GB = 1073741824;
    public static final long KB = 1024;
    public static final long MB = 1048576;
	
	public static String bytesIntoHumanReadable(long value) {
        long[] dividers = {GB, MB, KB, 1};
        String[] units = {"GB", "MB", "KB", "B"};
        if (value < 1) {
            value = 0;
        }
        for (int i = 0; i < dividers.length; i++) {
            long divider = dividers[i];
            if (value >= divider) {
                return format(value, divider, units[i]);
            }
        }
        return null;
    }

    private static String format(long value, long divider, String unit) {
        double result = (double) value;
        if (divider > 1) {
            result /= (double) divider;
        }
        return new DecimalFormat("#,##0.#").format(result) + " " + unit;
    }
	
	public static void delete(File file) {
        if (file.exists()) {
            if (file.isDirectory()) {
                for (File f : file.listFiles()) {
                    delete(f);
                }
            }
            file.delete();
        }
    }
}