cmake_minimum_required(VERSION 3.22)
project(Cosphere VERSION 1.1.0)

# Runtime estatico no Windows: o plugin nao exige o VC++ Redistributable instalado
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(Cosphere
    COMPANY_NAME "Danilo Pereira"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Dper
    PLUGIN_CODE Cphr
    FORMATS VST3 Standalone
    PRODUCT_NAME "Cosphere"
    VST3_CATEGORIES Instrument Synth)

target_sources(Cosphere PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp)

# Fonte do titulo (Redaction 50, licenca OFL). Opcional: se Assets/Redaction50.ttf
# existir (o workflow baixa sozinho), ela e embutida no plugin.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Redaction50.ttf")
    juce_add_binary_data(CosphereAssets SOURCES Assets/Redaction50.ttf)
    set_target_properties(CosphereAssets PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
    target_link_libraries(Cosphere PRIVATE CosphereAssets)
    target_compile_definitions(Cosphere PUBLIC COSPHERE_HAS_FONT=1)
endif()

target_compile_definitions(Cosphere PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1)

target_compile_options(Cosphere PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/fp:fast>)

target_link_libraries(Cosphere
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags)
