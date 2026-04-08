#!/usr/bin/env python3
"""
Affichage temps réel de la position du robot via port série.
Format attendu depuis la Nucléo : x,y,angle  (CSV, une ligne par mouvement)
Usage : python3 robot_position.py [PORT] [BAUDRATE]
        python3 robot_position.py /dev/ttyACM0 115200
"""

import sys
import threading
import serial
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np

# --- Config ---
PORT     = sys.argv[1] if len(sys.argv) > 1 else "/dev/ttyACM0"
BAUDRATE = int(sys.argv[2]) if len(sys.argv) > 2 else 115200

# --- Historique de positions ---
xs      = [0.0]
ys      = [0.0]
angles  = [0.0]
lock    = threading.Lock()
running = True

# --- Lecture série dans un thread séparé ---
def lire_serie():
    global running
    try:
        ser = serial.Serial(PORT, BAUDRATE, timeout=1)
        print(f"[OK] Connecté sur {PORT} à {BAUDRATE} baud")
    except serial.SerialException as e:
        print(f"[ERREUR] Impossible d'ouvrir {PORT} : {e}")
        print("Astuce WSL : vérifie avec  ls /dev/ttyACM*  ou  ls /dev/ttyUSB*")
        running = False
        return

    while running:
        try:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            if not line:
                continue
            parts = line.split(",")
            if len(parts) != 3:
                continue
            x, y, a = float(parts[0]), float(parts[1]), float(parts[2])
            with lock:
                xs.append(x)
                ys.append(y)
                angles.append(a)
            print(f"  x={x:.3f}m  y={y:.3f}m  angle={np.degrees(a):.1f}°")
        except (ValueError, serial.SerialException):
            pass
    ser.close()


# --- Flèche représentant le robot ---
def dessiner_robot(ax, x, y, angle, taille=0.05):
    dx = taille * np.cos(angle)
    dy = taille * np.sin(angle)
    ax.annotate(
        "", xy=(x + dx, y + dy), xytext=(x, y),
        arrowprops=dict(arrowstyle="-|>", color="royalblue", lw=2)
    )
    circle = plt.Circle((x, y), taille * 0.4, color="royalblue", zorder=5)
    ax.add_patch(circle)


# --- Boucle matplotlib ---
def main():
    global running

    thread = threading.Thread(target=lire_serie, daemon=True)
    thread.start()

    fig, ax = plt.subplots(figsize=(7, 7))
    fig.suptitle("Position du robot (m)", fontsize=13)
    plt.tight_layout(rect=[0, 0, 1, 0.95])

    while plt.fignum_exists(fig.number):
        with lock:
            x_copy = xs.copy()
            y_copy = ys.copy()
            a_copy = angles.copy()

        ax.clear()
        ax.set_aspect("equal")
        ax.grid(True, linestyle="--", alpha=0.4)
        ax.set_xlabel("X (m)")
        ax.set_ylabel("Y (m)")

        # Marge auto avec un minimum de 0.5m
        if len(x_copy) > 1:
            margin = 0.3
            xmin = min(x_copy) - margin
            xmax = max(x_copy) + margin
            ymin = min(y_copy) - margin
            ymax = max(y_copy) + margin
            span = max(xmax - xmin, ymax - ymin, 1.0)
            cx = (xmin + xmax) / 2
            cy = (ymin + ymax) / 2
            ax.set_xlim(cx - span / 2, cx + span / 2)
            ax.set_ylim(cy - span / 2, cy + span / 2)
        else:
            ax.set_xlim(-1, 1)
            ax.set_ylim(-1, 1)

        # Trajectoire
        ax.plot(x_copy, y_copy, "o-", color="lightsteelblue",
                markersize=4, linewidth=1.2, label="Trajectoire")

        # Point de départ
        ax.plot(x_copy[0], y_copy[0], "gs", markersize=8, label="Départ (0,0)", zorder=4)

        # Robot (dernière position)
        if a_copy:
            dessiner_robot(ax, x_copy[-1], y_copy[-1], a_copy[-1])

        # Coord courantes
        info = f"Pos actuelle : ({x_copy[-1]:.3f} m, {y_copy[-1]:.3f} m)\nAngle : {np.degrees(a_copy[-1]):.1f}°"
        ax.text(0.02, 0.98, info, transform=ax.transAxes,
                verticalalignment="top", fontsize=9,
                bbox=dict(boxstyle="round,pad=0.3", facecolor="white", alpha=0.7))

        ax.legend(loc="lower right", fontsize=8)
        plt.pause(0.2)

    running = False


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        running = False
        print("\n[Arrêt]")