cmake_minimum_required(VERSION 3.30.3)

set(CURRENT_VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_UNIVERSAL "Build universal binary for both ARM and Intel" ON)
if(BUILD_UNIVERSAL)
    set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
endif()
set(CMAKE_XCODE_GENERATE_SCHEME OFF)

set(COMPANY_NAME "SPEKADEV")
set(MANUFACTURER_CODE "SPKD")
set(PROJECT_NAME "WebViewReciprocalClipper")
set(PRODUCT_NAME "${PROJECT_NAME} ${CURRENT_VERSION}")
set(PRODUCT_CODE "WCRC")
set(BUNDLE_ID "com.${COMPANY_NAME}.${PROJECT_NAME}")
set(FORMATS VST3 Standalone)
set(VST3_CATEGORIES Distortion)

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

add_subdirectory(JUCE)

juce_add_plugin(
  ${PROJECT_NAME}
  VERSION                   "${CURRENT_VERSION}"
  COMPANY_NAME              "${COMPANY_NAME}"
  BUNDLE_ID                 "${BUNDLE_ID}"
  IS_SYNTH                  FALSE
  NEEDS_MIDI_INPUT          FALSE
  NEEDS_MIDI_OUTPUT         FALSE
  IS_MIDI_EFFECT            FALSE
  PLUGIN_MANUFACTURER_CODE  "${MANUFACTURER_CODE}"
  PLUGIN_CODE               "${PRODUCT_CODE}"
  FORMATS                   "${FORMATS}"
  VST3_CATEGORIES           "${VST3_CATEGORIES}"
  PRODUCT_NAME              "${PRODUCT_NAME}"
  COPY_PLUGIN_AFTER_BUILD   TRUE
)

juce_generate_juce_header(${PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
 
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE 
    juce::juce_audio_utils 
    juce::juce_dsp
  PUBLIC 
    juce::juce_recommended_config_flags 
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

target_compile_definitions(
  ${PROJECT_NAME}
  PUBLIC 
    JUCE_WEB_BROWSER=0 
    JUCE_USE_CURL=0 
    JUCE_VST3_CAN_REPLACE_VST2=0
    VERSION="${CURRENT_VERSION}"
    PRODUCT_NAME_WITHOUT_VERSION="${PROJECT_NAME}"
)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Werror -Wextra -Wpedantic -Wno-gnu-zero-variadic-macro-arguments)
endif()