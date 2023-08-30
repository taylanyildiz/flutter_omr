import 'dart:ffi';
import 'package:ffi/ffi.dart';

class OrmBindings {
  /// Holds the symbol lookup function.
  final Pointer<T> Function<T extends NativeType>(String symbolName) _lookup;

  /// The symbols are looked up in [dynamicLibrary].
  OrmBindings(DynamicLibrary dynamicLibrary) : _lookup = dynamicLibrary.lookup;

  /// The symbols are looked up with [lookup].
  OrmBindings.fromLookup(
    Pointer<T> Function<T extends NativeType>(String symbolName) lookup,
  ) : _lookup = lookup;

  late final _getOpenCVVersion =
      _lookup<NativeFunction<Pointer<Utf8> Function()>>('getOpenCVVersion')
          .asFunction<Pointer<Utf8> Function()>();
  late final _convertImageToGrayImage = _lookup<
          NativeFunction<
              Void Function(
                Pointer<Utf8>,
                Pointer<Utf8>,
              )>>('convertImageToGrayImage')
      .asFunction<
          void Function(
            Pointer<Utf8>,
            Pointer<Utf8>,
          )>();

  late final _readOpticalForm = _lookup<
          NativeFunction<
              Void Function(
                Pointer<Utf8>,
                Pointer<Utf8>,
                Int32,
                Int32,
                Pointer<Int32>,
              )>>('readOpticalForm')
      .asFunction<
          void Function(
            Pointer<Utf8>,
            Pointer<Utf8>,
            int,
            int,
            Pointer<Int32>,
          )>();

  String getOpenCVVersion() {
    return _getOpenCVVersion().cast<Utf8>().toDartString();
  }

  void convertImageToGrayImage(String inputPath, String outputPath) {
    _convertImageToGrayImage(
      inputPath.toNativeUtf8(),
      outputPath.toNativeUtf8(),
    );
  }

  void readOpticalForm(String inputPath, String outputPath, int questionCount,
      int answerCount, Pointer<Int32> markings) {
    _readOpticalForm(
      inputPath.toNativeUtf8(),
      outputPath.toNativeUtf8(),
      questionCount,
      answerCount,
      markings,
    );
  }
}
