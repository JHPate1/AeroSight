# AeroSight
### An Open-Source, Low-Cost Retroreflective Schlieren Imaging System

## Overview
AeroSight is a high-fidelity Schlieren imaging device built to make advanced aerodynamic and thermodynamic visualization actually accessible. Professional-grade Schlieren setups, the kind NASA uses easily runs past $5,000. AeroSight delivers 1080p visualization for a fraction of that, using a modular optical design and open-source hardware. Swap lenses, swap beam splitters, tweak the software. The whole thing is built for people who want to experiment like real lab equipment.
![Diagram](https://cdn.discordapp.com/attachments/1354640349103915079/1520223610780909648/CAMERA.png?ex=6a406a11&is=6a3f1891&hm=f3ee9704536a569a4b217f71ae4ef3d47b3bab0203d1968db6bce5911fc5695b&)
## How it looks inside
![Enclosure](https://cdn.discordapp.com/attachments/1513348059075969145/1520598872425238648/image.png?ex=6a41c78e&is=6a40760e&hm=9ea91b0a0ff12c99c7a3409be4e483fd4b16753a74e4292eee1d3c6234c2e9ec)
## Why AeroSight?
* **Cheap where it counts:** Total build cost is about $514.
* **Modular:** Upgrade optics (lens, NPBS cube) as your needs change.
* **Flexible software:** Raw video, real-color rendering, custom gradient mapping, all the good stuff.

## Project Contents
```/PCB/``` PCB files
```3D ``` 3D Printed Files

## Bill of Materials (BOM)

| Category | Item | Quantity | Total |
| :--- | :--- | :--- | :--- |
| **Processing** | Waveshare ESP32-P4-WIFI6 Development Board | 1 | $24.99 |
| **Camera** | Arducam OV5647 5MP Camera (CS Mount) | 1 | $25.99 |
| **Camera Cable** | Raspberry Pi CSI Ribbon Cable | 1 | $4.99 |
| **Optics** | 25 mm 50:50 Beam Splitter Cube | 1 | $94.95 |
| **Optics** | Eisco Plano-Convex Lens (50mm) | 2 | $19.98 |
| **Optics** | Front Surface Mirror 5.5" x 8" | 1 | $12.98 |
| **Illumination** | 3 W 3535 High-Power White LED | 1 | $10.29 |
| **Cooling** | 30 mm 5 V Brushless Fan Kit | 1 | $9.99 |
| **Cooling** | 20x20x10 mm Aluminum Heatsink | 1 | $7.99 |
| **Thermal** | ARCTIC MX-7 Thermal Paste | 1 | $6.89 |
| **Mechanical** | Metric Screw/Nut/Washer Kit | 1 | $13.98 |
| **Electrical** | 28 AWG Silicone Wire | 1 | $10.82 |
| **Electrical** | SPST Rocker Switch Kit | 1 | $8.99 |
| **Audio** | 3 W 8 Ω Mini Speaker | 1 | $9.99 |
| **Assembly** | WEP 946D IV Mini Reflow Hot Plate | 1 | $28.79 |
| **PCBWay** | PCB Fabrication & 3D Printing | 1 | $163.50 |
| **Shipping/Tax**| Combined Est. Tax/Shipping/Tariffs | - | $58.88 |
| **---** | **GRAND TOTAL** | --- | **$514.00** |

---

## Customization & Notes
* **Modular optics:** The optical path is built to swap. Change the primary lens or NPBS cube depending on your field of view or light sensitivity needs.
* **Fabrication:** The 3D-printed enclosure eats a big chunk of the budget. If you have access to a printer in-house, you can knock about $126.50 off the total.
