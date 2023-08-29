import 'dart:io';
import 'package:image_picker/image_picker.dart';

abstract class PickerHelper {
  PickerHelper._();

  /// Image picker obj
  static final ImagePicker imagePicker = ImagePicker();

  /// Picker Image
  static Future<File?> picker(ImageSource source) async {
    XFile? xFile = await imagePicker.pickImage(source: source);
    if (xFile == null) return null;
    return File(xFile.path);
  }
}
