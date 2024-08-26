using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Marker : MonoBehaviour
{
    private Camera mainCamera;
    private WorldGenerator generator;
    public Cell GetMarkedCell()
    {
        int x = Mathf.RoundToInt(transform.position.x);
        int y = Mathf.RoundToInt(transform.position.y);

        if (x > generator.world.cells.GetLength(0) || y > generator.world.cells.GetLength(1) || x <= 0 || y <= 0)
        {
            return null;
        }
        return generator.world.cells[x - 1, y - 1];
    }
    public Vector2Int GetMarkedCellPosition()
    {
        int x = Mathf.RoundToInt(transform.position.x);
        int y = Mathf.RoundToInt(transform.position.y);
        return new Vector2Int(x - 1, y - 1);
    }
    public CellInfo GetMarkedCellInfo()
    {
        return generator.cellInfo[GetMarkedCell().id];
    }
    void Start()
    {
        generator = GameObject.Find("generator").GetComponent<WorldGenerator>();
        mainCamera = Camera.main;
    }
    void Update()
    {
        Vector3 mousePos = Input.mousePosition;
        mousePos = mainCamera.ScreenToWorldPoint(new Vector3(mousePos.x, mousePos.y, mainCamera.transform.position.y - transform.position.y));
        transform.position = new Vector3(Mathf.RoundToInt(mousePos.x), Mathf.RoundToInt(mousePos.y), 0);
    }
}
