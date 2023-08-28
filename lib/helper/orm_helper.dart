import 'package:flutter_orm/models/orm_enums.dart';

abstract class ORMHelper {
  ORMHelper._();

  static AnswerType intToAnsserType(int value) {
    switch (value) {
      case 0:
        return AnswerType.A;
      case 1:
        return AnswerType.B;
      case 2:
        return AnswerType.C;
      case 3:
        return AnswerType.D;
      case 4:
        return AnswerType.E;
      case 5:
        return AnswerType.F;
      case 6:
        return AnswerType.empty;
      case 7:
        return AnswerType.error;
      default:
        return AnswerType.none;
    }
  }
}
