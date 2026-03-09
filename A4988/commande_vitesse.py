import math
from constantes import * 


def move_to_a_position(x_cible, y_cible, theta_cible):
    
    delta_X_field = x_cible - x_robot
    delta_Y_field = y_cible - y_robot
    omega = theta_cible - thetha_robot 

    x_robot = delta_X_field * math.cos(thetha_robot) + delta_Y_field * math.sin(thetha_robot)
    y_robot = -delta_X_field * math.sin(thetha_robot) + delta_Y_field * math.cos(thetha_robot)

    v1 = y_robot + x_robot - omega * (L + LAMBDA)
    v2 = y_robot - x_robot + omega * (L + LAMBDA)
    v3 = y_robot - x_robot - omega * (L + LAMBDA)
    v4 = y_robot + x_robot + omega * (L + LAMBDA)

    return v1, v2, v3, v4


def sent_to_motor(v1, v2, v3, v4):
    
    # conversion rad/s --> nb de pas pour moteur nema 14 
    nb_pas_v1 = NB_PAS_PAR_METRE * v1
    nb_pas_v2 = NB_PAS_PAR_METRE * v2
    nb_pas_v3 = NB_PAS_PAR_METRE * v3 
    nb_pas_v4 = NB_PAS_PAR_METRE * v4 

    # sent to motor