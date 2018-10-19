# ![Hedgehog Clock](https://raw.githubusercontent.com/jda0/hedgehog/gh-pages/assets/hedgehog_clock-vert-light.svg?sanitize=true)

Turn your Hedgehog Bot into a runaway alarm clock!
- Requires a [Hedgehog Bot](//hedgehogbot.co.uk/challenge2.html) and [PlatformIO](//platformio.org).
- [jamesin.space/hedgehog](https://jamesin.space/hedgehog) is the primary website for this project.

## Build

Install PlatformIO Core or PlatformIO IDE and build for `d1_mini`. The project follows the standard PlatformIO structure so all other settings should be fine at their defaults.

## Branches

- `master` contains the latest full code. Primary development will be done here.
- `starter` contains the workshop code skeleton, branched from the relevant `master` commit.
  - **Now compatible with Arduino IDE! Just open `src/src.ino`**
- `intermediate` and `staging` are reserved for future development.
- Use of other branches is not recommended.

## Contributing

- Please contribute through PRs.
- If contributing to `starter`, please ensure that any relevant changes to `master` are committed and parent your commit, i.e. `git checkout starter && git merge --no-ff --no-commit master`
- `starter:src/hh_clock.cpp` contains a lot of TODOs for workshop partipants to do, with lots of comments to help.
    - I hope this can be extended to `starter:src/hh_server.cpp` and some of the libs
