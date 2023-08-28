enum AnswerType {
  none(-1, '-'),
  A(0, 'A'),
  B(0, 'B'),
  C(0, 'C'),
  D(0, 'D'),
  E(0, 'E'),
  F(0, 'F'),
  empty(0, 'Empty'),
  error(0, 'Error');

  final int type;
  final String typeName;
  const AnswerType(this.type, this.typeName);
}
