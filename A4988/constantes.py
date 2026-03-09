import math

# Constantes -- HEADER 

# Dimenssion robot (voir schéma) 
L = 0.15          # distance centre - axe 
LAMBDA = 0.12     # largeur centre - axe
R = 0.04          # rayon des roues

# Nema 14 
STEPS_PER_REV = 200     # 360 / 1.8 
MICROSTEPPING = 16      # A4988
NB_PAS_PAR_METRE = (STEPS_PER_REV * MICROSTEPPING) / (2 * math.pi * R)

# Globale varaible pour avoir acces à la position du robot 
global x_robot
global y_robot
global thetha_robot
