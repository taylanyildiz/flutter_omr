enum AnswerType {
  none(-1, '-'),
  A(0, 'A'),
  B(1, 'B'),
  C(2, 'C'),
  D(3, 'D'),
  E(4, 'E'),
  F(5, 'F'),
  empty(6, 'Empty'),
  error(7, 'Error');

  final int type;
  final String typeName;
  const AnswerType(this.type, this.typeName);
}
