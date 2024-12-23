package com.mobili.usbcamera.utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class FileReaderUtils {
    public static String readMobiliIdFromExternalStorage() {
        String filePath = "/storage/emulated/0/Documents/mobili_id.txt";
        StringBuilder stringBuilder = new StringBuilder();
        try {
            File file = new File(filePath);
            if (!file.exists()) {
                // File does not exist, return an empty string
                System.out.println("File does not exist: " + filePath);
                return "mobilixxx";
            }

            BufferedReader reader = new BufferedReader(new FileReader(file));
            String line;
            while ((line = reader.readLine()) != null) {
                stringBuilder.append(line).append("\n");
            }
            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        String mobiliId = stringBuilder.toString();
        mobiliId = mobiliId.replace("\n", "").replace("\r", "");
        return mobiliId;
    }
}
