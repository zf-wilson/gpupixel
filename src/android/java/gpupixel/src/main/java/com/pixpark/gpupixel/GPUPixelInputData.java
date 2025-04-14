package com.pixpark.gpupixel;

import android.content.Context;

import androidx.annotation.NonNull;

public class GPUPixelInputData extends GPUPixelSource {

    int textureId;

    public GPUPixelInputData(@NonNull Context context) {
        if (mNativeClassID != 0) return;

        GPUPixel.setContext(context);

        GPUPixel.getInstance().runOnDraw(() -> {
            mNativeClassID = GPUPixel.nativeInputData();
        });
    }

    public void processData(int textureId, int width, int height) {
        GPUPixel.getInstance().runOnDraw(() -> {
            GPUPixel.nativeInputDataProcess(mNativeClassID, textureId, width, height);
        });
    }

}
