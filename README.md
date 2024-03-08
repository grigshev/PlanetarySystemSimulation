# Brief
The program simulates the formation of a planetary system around a star from a disk of gas and dust.
The simulation lasts about 15 minutes. During this time, planets form from the cloud of the protoplanetary disk and evolve to a certain type, which depends on the mass, composition and distance from the star. During the simulation, a system of planets of different types, comets and asteroids is formed.
It may be similar to the Solar System, or it may be very different, as it depends on random factors.

<img src="https://github.com/grigshev/PlanetarySystemSimulation/blob/master/assets/images/presentation.png" alt="Screenshot">

# Object types
- star
- gas clouds consist of light elements (hydrogen and other gases, water, ammonia, methane, etc).
- dust clouds consist of heavy elements (metals and silicates).
- comets - small objects consisting only of light elements.
- asteroids - small objects of mixed composition.
- dwarf planets - small planets like Pluto or Ceres
- earth-like planets, there are 4 types:
  - rocky (like Mars, Venus, Mercury)
  - oceanic (like Earth)
  - hot (there are no similar ones in the Solar System)
  - icy (there are no similar ones in the Solar System)
- gas subgiants (like Uranus and Neptune)
- gas giants (like Jupiter and Saturn)

# Physical model
The program simulates the following astrophysical laws and phenomena:
1. <b>Gravity</b>. Celestial bodies move in orbits, gravitational influence each other, changing orbits.
They can collide with each other, with larger planets increasing in size and mass as they collide with smaller planets, comets and asteroids.
2. <b>Chemical composition</b>. Celestial bodies have different proportions of light and heavy components, which determines their mass, volume, type and displayed color.
3. <b>Light pressure</b>. It has a stronger effect on gas clouds than on dust clouds, which leads to the formation of a “snow line” and the predominance of rocky planets in close orbits from the star.
4. <b>Temperature</b>. It depends on the distance to the star. Also, during a collision, celestial bodies become very hot, but gradually cool down to equilibrium temperature. Temperature affects the displayed color of the planet.
5. <b>Late heavy bombardment</b>. At the initial stages of the formation of a planetary system, rocky planets in close orbits, as a rule, do not contain light elements (water), since they are located inside the “snow line”.
However, in the later stages they may collide with comets from distant regions, which orbits was changed when approaching the giant planet.
This is the mechanism for the appearance of oceans on Earth and it happens quite often randomly in this simulation.