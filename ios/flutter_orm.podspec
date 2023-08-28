#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint flutter_orm.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'flutter_orm'
  s.version          = '0.0.1'
  s.summary          = 'A new Flutter FFI plugin project.'
  s.description      = <<-DESC
A new Flutter FFI plugin project.
                       DESC
  s.homepage         = 'http://example.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Your Company' => 'email@example.com' }

  # This will ensure the source files in Classes/ are included in the native
  # builds of apps using this FFI plugin. Podspec does not support relative
  # paths, so Classes contains a forwarder C file that relatively imports
  # `../src/*` so that the C sources can be shared among all target platforms.
  s.source           = { :path => '.' }
  s.source_files = 'Classes/**/*'
  s.dependency 'Flutter'
  s.platform = :ios, '11.0'

  # Flutter.framework does not contain a i386 slice.
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES', 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'i386' }
  s.swift_version = '5.0'

  s.prepare_command = <<-CMD
    # Unzip File
    unzip -o "opencv2.framework.zip" -d "${PODS_TARGET_SRCROOT}"
    # Delete Zip File
    # rm "opencv2.framework.zip"
  CMD

  # Tell CocoaPods not to remove framework
  s.preserve_paths = 'opencv2.framework'

  # Include OpenCV framework
  s.vendored_frameworks = 'opencv2.framework'

  # C++ include
  s.library = "c++"
end