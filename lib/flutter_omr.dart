import 'dart:ffi';
import 'package:ffi/ffi.dart';
import '/helper/orm_helper.dart';
import 'models/models.dart';
import 'src/omr.dart';
import 'src/omr_bindings_generated.dart';

abstract class FlutterOMR {
  FlutterOMR._();

  /// The bindings to the native functions in [_dylib].
  static final OrmBindings _bindings = OrmBindings(dylib);

  /// Open-CV Version
  static String get version => _bindings.getOpenCVVersion();

  /// Optical read
  static EntOrm onOptic(int questionCount, int answerCount, String imagePath) {
    EntOrm orm = EntOrm(
      questionCount: questionCount,
      answerCount: answerCount,
      imagePath: imagePath,
    );

    List<String> output = imagePath.split('.');

    output[output.length - 2] = "${output[output.length - 2]}_gray";

    Pointer<Int32> resultBuffer = malloc.allocate<Int32>(
      sizeOf<Int32>() * questionCount,
    );

    _bindings.readOpticalForm(
      imagePath,
      output.join("."),
      questionCount,
      answerCount,
      resultBuffer,
    );

    for (var i = 0; i < questionCount; i++) {
      List<ORMQuestion> answers = [...orm.answers];
      final type = OMRHelper.intToAnsserType(resultBuffer.elementAt(i).value);
      answers[i] = answers[i].copy(answerType: type);
      orm = orm.copy(answers: answers);
    }

    malloc.free(resultBuffer);
    orm = orm.copy(imagePath: output.join('.'));

    return orm;
  }
}
