{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    vulkan-validation-layers
    
    cmake
    ninja
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
  ]);

  shellHook = ''
    export CMAKE_GENERATOR=Ninja
  '';
}
