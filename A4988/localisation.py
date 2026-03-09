import math
import time
from constantes import * 





def get_gyro_theta():
    return 0 

def update_position(steps_moteurs):
    
    dist_roues = [(s / STEPS_PER_REV) * (2 * math.pi * R) for s in steps_moteurs]
    
    dy_robot = (dist_roues[0] + dist_roues[1] + dist_roues[2] + dist_roues[3]) / 4
    dx_robot = (dist_roues[0] - dist_roues[1] - dist_roues[2] + dist_roues[3]) / 4
    
    theta_nouveau = get_gyro_theta()
    
    delta_X_terrain = dx_robot * math.cos(theta_nouveau) - dy_robot * math.sin(theta_nouveau)
    delta_Y_terrain = dx_robot * math.sin(theta_nouveau) + dy_robot * math.cos(theta_nouveau)
    
    x_robot += delta_X_terrain
    y_robot += delta_Y_terrain
    theta_robot = theta_nouveau

    print(f"Position : X={x_robot:.3f}m, Y={y_robot:.3f}m, Theta={math.degrees(theta_robot):.1f}°")


if __name__ == "__main__":


    pas_derniere_boucle = [1000, 1080, 1000, 1000] 
    update_position(pas_derniere_boucle)