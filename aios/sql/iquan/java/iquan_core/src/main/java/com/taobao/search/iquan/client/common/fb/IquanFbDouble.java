// automatically generated by the FlatBuffers compiler, do not modify

package com.taobao.search.iquan.client.common.fb;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class IquanFbDouble extends Table {
  public static IquanFbDouble getRootAsIquanFbDouble(ByteBuffer _bb) { return getRootAsIquanFbDouble(_bb, new IquanFbDouble()); }
  public static IquanFbDouble getRootAsIquanFbDouble(ByteBuffer _bb, IquanFbDouble obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; }
  public IquanFbDouble __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public double value() { int o = __offset(4); return o != 0 ? bb.getDouble(o + bb_pos) : 0.0; }

  public static int createIquanFbDouble(FlatBufferBuilder builder,
      double value) {
    builder.startObject(1);
    IquanFbDouble.addValue(builder, value);
    return IquanFbDouble.endIquanFbDouble(builder);
  }

  public static void startIquanFbDouble(FlatBufferBuilder builder) { builder.startObject(1); }
  public static void addValue(FlatBufferBuilder builder, double value) { builder.addDouble(0, value, 0.0); }
  public static int endIquanFbDouble(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}

