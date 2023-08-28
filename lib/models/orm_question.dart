import '/models/orm_enums.dart';

class ORMQuestion {
  final int questionIndex;
  final AnswerType answerType;

  ORMQuestion({
    required this.questionIndex,
    required this.answerType,
  });

  ORMQuestion copy({
    int? questionIndex,
    AnswerType? answerType,
  }) {
    return ORMQuestion(
      questionIndex: questionIndex ?? this.questionIndex,
      answerType: answerType ?? this.answerType,
    );
  }

  @override
  String toString() => """
    Answer Index: $questionIndex,
    Answer Type: ${answerType.typeName}
  """;
}
