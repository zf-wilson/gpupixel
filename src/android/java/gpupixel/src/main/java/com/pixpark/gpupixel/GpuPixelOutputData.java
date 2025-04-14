package com.pixpark.gpupixel;

public class GpuPixelOutputData extends GPUPixelSource implements GPUPixelSink {

    public GpuPixelOutputData() {
        if (mNativeClassID != 0) return;
        GPUPixel.getInstance().runOnDraw(() -> {
            mNativeClassID = GPUPixel.nativeOutputData();
        });
    }

    public int getOutputTextureId() {
        return GPUPixel.nativeOutputDataGetTextureId(mNativeClassID);
    }

    @Override
    public long getNativeClassID() {
        return mNativeClassID;
    }
}
