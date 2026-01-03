# MPU6050


### Cablage 

| Uno | MPU6050| 
|-----|--------|
| V5  | VCC    | 
| GND | GND    | 
| A5  | SCL    | 
| A4  | SDA    | 

## Gyroscope 

### Calibration 

Le gyroscope n'est pas directement précis (il pense que 0°/sec = 2°/sec quand il est immobile)

On calibre : 
- On le fait boucler 2000 fois sans le faire bouger (2 secondes)
- On calcule la moyenne de l'erreur précise du capteur
- On soustrait cette erreur à chaque mesure d'angle

### Obtenir angle 

Angle recheché : Yaw (rotation)

````
Angle = Angleinit + (Vitesse * Temps)
```` 
- Angle recheché 
- Angleinit initiale (à 0)
- Vitesse de rotation (en °/sec) donné par le gyroscope
- Temps (en sec) d'une boucle 

### Sensibilité des données extraites

| Valeur Hex  | Plage de mesure  | Diviseur  | 
| :---: | :---: | :---: | 
| `0x00` | ± 250°/s | 131.0 | 
| `0x08` | ± 500°/s | 65.5 |  
| `0x10` | ± 1000°/s | 32.8 | 
| `0x18` | ± 2000°/s | 16.4 | 

### Résultats 



## Accéléromètre