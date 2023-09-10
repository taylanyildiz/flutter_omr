import 'dart:developer';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter_omr/flutter_omr.dart';
import 'package:flutter_omr/models/models.dart';
import 'package:flutter_omr_example/picker_helper.dart';
import 'package:image_picker/image_picker.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Flutter ORM Example',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const HomePage(title: 'Flutter ORM'),
    );
  }
}

class HomePage extends StatefulWidget {
  const HomePage({
    super.key,
    required this.title,
  });

  /// Title of page
  final String title;

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  /// Open-CV Version
  String? openCvVersion;

  EntOrm? orm;

  /// Image file
  File? file;

  @override
  void initState() {
    openCvVersion = FlutterOMR.version;
    log("OPEN-CV Version $openCvVersion");
    super.initState();
  }

  void _onPickImage() async {
    final file = await PickerHelper.picker(ImageSource.gallery);
    if (file == null) return;
    setState(() => this.file = file);
  }

  void _onReadImage() async {
    if (file == null) return;
    orm = FlutterOMR.onOptic(10, 5, file!.path);
    setState(() => file = File(orm!.imagePath));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text(widget.title)),
      body: Column(
        children: [
          Expanded(child: _buildImage),
          if (orm != null)
            Container(
              color: Colors.transparent,
              width: double.infinity,
              height: 40.0,
              child: ListView.separated(
                separatorBuilder: (context, index) => const SizedBox(
                  width: 5.0,
                ),
                scrollDirection: Axis.horizontal,
                itemCount: orm!.questionCount,
                itemBuilder: (context, index) {
                  final answerType = orm!.answers[index];
                  return Text("${index + 1}: ${answerType.answerType}");
                },
              ),
            ),
          _buildPickButton,
          _buildReadButton,
        ],
      ),
    );
  }

  Widget get _buildImage {
    return Container(
      decoration: BoxDecoration(border: Border.all(width: 1.0)),
      child: Stack(
        children: [
          Center(
            child: Visibility(
              visible: file == null,
              child: const Text("Please Select Image"),
            ),
          ),
          if (file != null) ...[
            Image.file(file!),
          ]
        ],
      ),
    );
  }

  Widget get _buildPickButton {
    return ElevatedButton(
      onPressed: _onPickImage,
      style: ButtonStyle(
        shape: MaterialStateProperty.all(
          RoundedRectangleBorder(borderRadius: BorderRadius.circular(4.0)),
        ),
      ),
      child: const Text("Pick Image"),
    );
  }

  Widget get _buildReadButton {
    return Visibility(
      visible: file != null,
      child: ElevatedButton(
        onPressed: _onReadImage,
        style: ButtonStyle(
          shape: MaterialStateProperty.all(
            RoundedRectangleBorder(borderRadius: BorderRadius.circular(4.0)),
          ),
        ),
        child: const Text("Read Image"),
      ),
    );
  }
}
