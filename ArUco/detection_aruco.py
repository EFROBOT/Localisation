"""
Detection de marqueurs ArUco a l'aide d'une camera sur Raspberry Pi.

Dependances :
    pip install opencv-contrib-python picamera2

Utilisation :
    python detection_aruco.py
"""

import cv2

# --------------- Configuration ---------------

# Dictionnaire ArUco utilise pour la detection
ARUCO_DICT_NAME = cv2.aruco.DICT_4X4_100

# Resolution de la camera
CAMERA_WIDTH = 640
CAMERA_HEIGHT = 480

# Couleur pour l'affichage (BGR)
COULEUR_ID = (0, 0, 255)


def initialiser_camera():
    """Initialise la camera.

    Essaie d'abord Picamera2 (camera native Raspberry Pi),
    puis se rabat sur OpenCV VideoCapture.
    """
    try:
        from picamera2 import Picamera2

        picam2 = Picamera2()
        config = picam2.create_preview_configuration(
            main={"size": (CAMERA_WIDTH, CAMERA_HEIGHT), "format": "RGB888"}
        )
        picam2.configure(config)
        picam2.start()
        return picam2, "picamera2"
    except (ImportError, RuntimeError):
        cap = cv2.VideoCapture(0)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT)
        if not cap.isOpened():
            raise RuntimeError("Impossible d'ouvrir la camera.")
        return cap, "opencv"


def lire_image(camera, type_camera):
    """Lit une image depuis la camera.

    Retourne l'image BGR ou None en cas d'echec.
    """
    if type_camera == "picamera2":
        frame = camera.capture_array()
        return cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)

    ret, frame = camera.read()
    if not ret:
        return None
    return frame


def fermer_camera(camera, type_camera):
    """Ferme la camera proprement."""
    if type_camera == "picamera2":
        camera.stop()
    else:
        camera.release()


def detecter_aruco(image, detecteur):
    """Detecte les marqueurs ArUco dans une image.

    Retourne les coins, les identifiants et les marqueurs rejetes.
    """
    coins, ids, rejetes = detecteur.detectMarkers(image)
    return coins, ids, rejetes


def dessiner_marqueurs(image, coins, ids):
    """Dessine les contours et identifiants des marqueurs detectes."""
    if ids is None:
        return image

    cv2.aruco.drawDetectedMarkers(image, coins, ids)

    for i, coin in enumerate(coins):
        # Centre du marqueur
        c = coin[0]
        cx = int(c[:, 0].mean())
        cy = int(c[:, 1].mean())
        marker_id = int(ids[i][0])

        cv2.putText(
            image,
            f"ID: {marker_id}",
            (cx - 20, cy - 15),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.6,
            COULEUR_ID,
            2,
        )

    return image


def main():
    """Boucle principale de detection ArUco."""
    # Initialisation du dictionnaire et du detecteur ArUco
    dictionnaire = cv2.aruco.getPredefinedDictionary(ARUCO_DICT_NAME)
    params = cv2.aruco.DetectorParameters()
    detecteur = cv2.aruco.ArucoDetector(dictionnaire, params)

    # Initialisation de la camera
    camera, type_camera = initialiser_camera()
    print(f"Camera initialisee ({type_camera}). Appuyez sur 'q' pour quitter.")

    ids_precedents = set()

    try:
        while True:
            image = lire_image(camera, type_camera)
            if image is None:
                print("Erreur de lecture de l'image.")
                break

            # Detection
            coins, ids, _ = detecter_aruco(image, detecteur)

            # Affichage
            image = dessiner_marqueurs(image, coins, ids)

            ids_actuels = set(int(mid[0]) for mid in ids) if ids is not None else set()
            if ids_actuels != ids_precedents:
                if ids_actuels:
                    print(f"Marqueurs detectes : {sorted(ids_actuels)}")
                elif ids_precedents:
                    print("Aucun marqueur detecte.")
                ids_precedents = ids_actuels

            cv2.imshow("Detection ArUco", image)

            if cv2.waitKey(1) & 0xFF == ord("q"):
                break
    finally:
        fermer_camera(camera, type_camera)
        cv2.destroyAllWindows()
        print("Camera fermee.")


if __name__ == "__main__":
    main()
