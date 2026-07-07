{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    # Build tools
    cmake
    neocmakelsp
    ninja
    pkg-config
    clang
    clang-tools
    gdb

    sdl3

    # OpenGL / rendering
    mesa
    libGL
    vulkan-loader
    vulkan-headers

    # Wayland stack
    wayland
    wayland-protocols
    libxkbcommon
    libdecor
    libdrm

    # X11 stack
    libX11
    libxcb
    libXext
    libXcursor
    libXi
    libXtst
    libXfixes
    libXrandr
    libXScrnSaver

    # Audio / hotplug / runtime integration
    alsa-lib
    libpulseaudio
    pipewire
    dbus
    systemd   # provides libudev

    # Often useful for SDL internals/optimizations
    libffi
    nasm
  ];

  shellHook = ''
    export CC=${pkgs.stdenv.cc}/bin/cc
    export CXX=${pkgs.stdenv.cc}/bin/c++
  '';
}
