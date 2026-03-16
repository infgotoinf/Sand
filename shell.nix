{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    vulkan-validation-layers
    
    alsa-lib
    libdecor
    libusb1
    libxkbcommon
    vulkan-loader
    wayland
    libX11
    libX11.dev
    libXext
    libXi
    udev

    libxcursor
    libxrandr
    libxscrnsaver
    libxcb
    libxtst

    cmake
    ninja
    freetype
    
    doxygen
    graphviz
    plantuml
    jdk
  ];

  LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (with pkgs; [
    alsa-lib
    libdecor
    libusb1
    libxkbcommon
    vulkan-loader
    wayland
    libX11
    libX11.dev
    libXext
    libXi
    udev

    SDL2
  ]);

  shellHook = ''
    export CMAKE_GENERATOR=Ninja
  '';
}
