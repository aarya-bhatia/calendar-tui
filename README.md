# Calendar TUI

A simple terminal-based calendar application written in C++ as a learning project. It provides a daily/monthly view of events and allows managing them directly from the terminal.

## Features

- Interactive calendar grid with navigation.
- Event management (add/delete events).
- Persistent storage for events.
- Logging for debugging and application state tracking.
- Visual feedback using `ncurses` colors.

## Architecture

The project follows a modular design:
- **`Application`**: Main controller that manages the application lifecycle, `ncurses` initialization, and the main loop.
- **`AppState`**: Manages the core data, selected dates, and event persistence logic.
- **`View` (and sub-classes)**: Handles rendering of different UI components (Header, Calendar grid, Events list, Footer) using a decoupled view-model approach.
- **`Util`**: Contains helper functions for date manipulation, logging, and network operations.

## Dependencies

The project relies on the following libraries:
- `libncurses`: For terminal UI rendering and input handling.
- `libcurl`: For networking capabilities (e.g., push notifications).

## Building and Running

### Compile
To build the application, use the provided `Makefile`:
```bash
make
```

### Run
Launch the application by executing the compiled binary:
```bash
./main
```

### Logging
Application logs are written to:
- `app.log`: Contains initialization details, keypresses, and event operations.

### Data Storage
Events are stored as individual files in the `./data/` directory. Each file represents an event, named using a timestamp-based convention.

## License
MIT
