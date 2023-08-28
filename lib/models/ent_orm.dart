import '/models/orm_question.dart';
import '/models/orm_enums.dart';

class EntOrm {
  final int questionCount;
  final int answerCount;
  final String imagePath;
  final List<ORMQuestion> answers;

  /// Constructor of [EntOrm]
  EntOrm({
    required this.questionCount,
    required this.answerCount,
    required this.imagePath,
    List<ORMQuestion>? answers,
  }) : answers = answers ?? _generate(questionCount);

  /// Copy [EntOrm]
  EntOrm copy({
    int? questionCount,
    int? answerCount,
    String? imagePath,
    List<ORMQuestion>? answers,
  }) {
    return EntOrm(
      questionCount: questionCount ?? this.questionCount,
      answerCount: answerCount ?? this.answerCount,
      imagePath: imagePath ?? this.imagePath,
      answers: answers ?? this.answers,
    );
  }

  @override
  String toString() => """
    Question Count: $questionCount,
    Answer Count: $answerCount,
    Image Path: $imagePath,
    Answers: ${answers.map((e) => e.toString())}
  """;
}

/// Generate ORM Questions
List<ORMQuestion> _generate(int answerCount) {
  return List.generate(answerCount, (index) {
    return ORMQuestion(questionIndex: index + 1, answerType: AnswerType.none);
  });
}
