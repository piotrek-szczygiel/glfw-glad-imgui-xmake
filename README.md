# Building

## Requirements

- C++ compiler
- xmake

You can install [xmake](https://xmake.io/) on your machine using these one-liners:

```bash
# via curl
bash <(curl -fsSL https://xmake.io/shget.text)

# via wget
bash <(wget https://xmake.io/shget.text -O -)

# via powershell
Invoke-Expression (Invoke-Webrequest 'https://xmake.io/psget.text' -UseBasicParsing).Content
```

If you are on Linux you might also need OpenGL and X11 development libraries.

## Usage

```bash
# Optionally: use debug mode
xmake config -m debug

# Build application
xmake

# Run application and set working directory to project root
xmake run -w .

# Format code
xmake format
```
