# Link to Source Repository  
https://github.com/StevenvdHoven/DAE_GameEngine

---

# DAE Game Engine

The **DAE Game Engine** is a component-based, single-scene game engine inspired by Unity. It is designed for simplicity and flexibility, offering essential systems such as rendering, audio, scene management, and a built-in level editor.

---

## Engine Overview

This engine adopts a **component-based architecture** similar to Unity. Everything—from rendering to game logic—is handled via components attached to `GameObject` instances. The engine supports a **single active scene**, which is cleared before loading or creating a new one.

---

## Components

Components are central to the engine and responsible for most runtime behaviors:

- Each component is tied to a `GameObject` and cannot exist independently.
- Supports serialization and deserialization—users must override the relevant functions.
- Custom components can be created and registered for use at runtime or in the level editor.

---

## Level Editor

Press `F4` at runtime to open the built-in **Level Editor**. With the editor, you can:

- Create and organize `GameObjects`
- Attach built-in or custom components (if registered through the `ComponentRegistry`)
- Save and load scenes and prefabs

---

## Observer Pattern

The engine uses a basic **observer-subject** system:

- Subjects notify observers by passing a reference to a `GameObject`.
- `nullptr` can be returned if it's guaranteed only one subject is being observed.
- Best practice is to always return the originating `GameObject` for clarity and reliability.

---

## Factory Pattern & Component Registry

The engine supports scene and prefab saving/loading using a **factory pattern** via the `ComponentRegistry`:

- Register custom components to allow instantiation from serialized data
- Enables the Level Editor to list and instantiate these components dynamically

---

## Threaded Sound System

Audio is handled on a **dedicated thread** for performance:

- Thread-safe with mutexes and condition variables
- Sounds are centrally managed and identified via `SoundID`s
- The audio thread processes queued sound events asynchronously

---

## Graph Editor & Pathfinding

A lightweight **Graph Editor** and simple **PathFinding** system are included:

- Rapidly create, edit, save, and load graph-based maps
- Integrated pathfinding system for small-scale navigation
- Designed for games requiring basic AI movement or grid-based logic




