using UnityEngine;

public class CameraController : MonoBehaviour
{
    public Vector2 maxBorders;
    public Vector2 minBorders;

    public Camera cam;
    public float zoomSpeed = 10f;
    public float moveSpeed = 0.5f;
    public float minZoom = 5f;
    public float maxZoom = 15f;
    private Vector3 dragOrigin;

    void Update()
    {
        // «ум камеры
        float scroll = Input.GetAxis("Mouse ScrollWheel");
        cam.orthographicSize -= scroll * zoomSpeed;
        cam.orthographicSize = Mathf.Clamp(cam.orthographicSize, minZoom, maxZoom);

        // ѕеремещение камеры
        if (Input.GetMouseButtonDown(1)) //  нопка колесика мыши
        {
            dragOrigin = cam.ScreenToWorldPoint(Input.mousePosition);
        }

        if (Input.GetMouseButton(1))
        {
            Vector3 difference = dragOrigin - cam.ScreenToWorldPoint(Input.mousePosition);
            cam.transform.position += difference * moveSpeed;
            cam.transform.position = new Vector3(Mathf.Clamp(cam.transform.position.x, minBorders.x, maxBorders.x), Mathf.Clamp(cam.transform.position.y, minBorders.y, maxBorders.y), cam.transform.position.z);
        }
    }
}
