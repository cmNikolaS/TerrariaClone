# Terraria Clone

A 2D sandbox/mining game inspired by Terraria, written in C++17 with SFML 3.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![SFML](https://img.shields.io/badge/SFML-3.0-green)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)


<img width="1536" height="800" alt="Screenshot 2026-04-20 22-47-17" src="https://github.com/user-attachments/assets/64df2844-96e8-4fc6-8529-95b3013930d9" />


## Features

- **Procedurally generated world** — random-walk heightmap with surface, subsurface, ores and cave worms
- **Biomes** — snow biome at higher altitudes (SnowGrass / SnowDirt)
- **Ores** — Coal, Iron, Gold, Diamond with rarity-weighted placement and clustered veins
- **Day / Night cycle** — darkness overlay that smoothly fades using a sine curve
- **Enemies** — zombies spawn at night, chase the player, handle gravity and platform jumping
- **Player physics** — AABB tile collision, gravity, jump, water slowdown
- **Creative mode** — disable gravity and fly freely
- **Hotbar** — 9 slots, selectable via number keys, mouse scroll, or click
- **World map view** — zoom out to see the full world
- **Restartable worlds** — press R for a fresh seed without relaunching

<img width="1889" height="1199" alt="image" src="https://github.com/user-attachments/assets/4db0aa7b-eb0e-47c4-ace3-f4eda2fd5656" />

## Controls

| Key | Action |
|-----|--------|
| A / D | Move left / right |
| Space | Jump (or fly up in creative) |
| S | Fly down (creative mode only) |
| 1-9 | Select hotbar slot |
| Mouse Scroll | Navigate hotbar |
| Left Click | Destroy block |
| Right Click | Place block |
| V | Toggle world map view |
| C | Toggle creative mode |
| R | Regenerate world |
| Esc | Menu / Game |
| Q | Quit |

## Requirements

- **SFML 3.0** installed at `C:/SFML/` (include in `C:/SFML/include`, libs in `C:/SFML/lib`)
- **Visual Studio 2022** (MSBuild, platform toolset v145)
- **C++17** compiler support
- **Windows** (Win32 or x64)

## Building

1. Clone the repo:
   ```bash
   git clone <repo-url>
   cd TERRARIA
# TerrariaClone
