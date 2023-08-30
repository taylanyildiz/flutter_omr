#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint flutter_omr.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'flutter_omr'
  s.version          = '0.0.1'
  s.summary          = 'Flutter Optical Mark Recognition (OMR).'
  s.description      = 'Flutter Optical Reader'
  s.homepage         = 'http://example.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Taylan YILDIZ' => 'taylnyldiz@gmail.com' }


  s.source           = { :path => '.' }
  s.source_files = 'Classes/**/*'
  s.dependency 'Flutter'
  s.platform = :ios, '11.0'

  # Flutter.framework does not contain a i386 slice.
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES', 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'i386' }
  s.swift_version = '5.0'

  # Tell CocoaPods not to remove framework
  s.preserve_paths = 'opencv2.framework'

  # Include OpenCV framework
  s.vendored_frameworks = 'opencv2.framework'

  # Opecv-CV Source Git
  s.source           = { :http => "https://github.com/opencv/opencv/releases/download/4.5.3/opencv-4.5.3-ios-framework.zip",
  :sha256 => '10387024f4c5e353c01a455a6b1bdf33d81b9d7da7654b83c256834eeba9eab5' }

  # Download Open-CV and Unzip
  s.prepare_command = <<-CMD
    curl -O -L "https://github.com/opencv/opencv/releases/download/4.5.3/opencv-4.5.3-ios-framework.zip"
    unzip -o opencv-4.5.3-ios-framework.zip
    rm opencv-4.5.3-ios-framework.zip
    CMD


  # C++ include
  s.library = "c++"
end