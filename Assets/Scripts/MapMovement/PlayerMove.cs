using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
public class PlayerMove : MonoBehaviour
{
    public Vector2Int position;

    [HideInInspector]
    public MercenarySquadPanel squadPanel;

    [HideInInspector]
    public WorldGenerator generator;
    [HideInInspector]
    public List<Vector2Int> path;
    private Marker marker;

    [HideInInspector]
    public Cell goal;

    private Vector2Int goalPosition;

    float timerToMove = 1f;
    float timeToNextCell = 0.8f;

    Camera cameraCurr;
    EventController eventController;
    DayTimer timer;
    Squad squad;

    AudioSource sounder;
    public AudioClip moveSound;

    public List<int> tryToAvoid = new List<int> { 0 };
    void Start()
    {
        timer = GameObject.Find("timer").GetComponent<DayTimer>();
        squadPanel = GameObject.Find("squadPanel").GetComponent<MercenarySquadPanel>();
        generator = GameObject.Find("generator").GetComponent<WorldGenerator>();
        marker = GameObject.Find("marker").GetComponent<Marker>();
        eventController = GameObject.Find("Eventer").GetComponent<EventController>();
        squad = GameObject.Find("squad").GetComponent<Squad>();
        sounder = GameObject.Find("SoundManager").GetComponent<AudioSource>();
        cameraCurr = Camera.main;
    }

    public void TapExplainButton()
    {
        eventController.ExlplainCell(goal);
    }
    public void UpdatePositionDisplay()
    {
        transform.position = new Vector3(position.x + 1, position.y + 1, transform.position.z);
    }
    void Update()
    {
        float playerMarkerScale = (cameraCurr.orthographicSize <= 3.5f) ? 1f : (cameraCurr.orthographicSize / 3f);
        transform.localScale = new Vector3(playerMarkerScale, playerMarkerScale, 1f);
        if (path != null && path.Count > 0 && !eventController.eventGoingNow)
        {
            if(timerToMove <= 0)
            {
                var newPos = path[0];
                position = newPos;
                squad.data.position = new Vec2(position);

                Cell currentCell = generator.world.cells[newPos.x, newPos.y];
                eventController.PassingCell(currentCell);
                timer.SpendTime(generator.GetCellInfo(currentCell).daysToMove);

                path.Remove(newPos);
                squadPanel.UpdateText(goal, path);
                generator.helperMap.SetTile(new Vector3Int(newPos.x, newPos.y, 0), null);
                if (path.Count == 0)
                {
                    generator.ClearHelperMap();
                }
                UpdatePositionDisplay();
                timerToMove = timeToNextCell;
                sounder.PlayOneShot(moveSound);

                Cell currentGoal = generator.world.cells[goalPosition.x, goalPosition.y];
                if (goal != currentGoal)
                {
                    SetGoal(currentGoal);
                }
            }
            else
            {
                timerToMove -= Time.deltaTime;
            }
        }
        if (Input.GetMouseButtonDown(0))
        {
            if (EventSystem.current.IsPointerOverGameObject())
                return;

            Cell markedCell = marker.GetMarkedCell();
            goalPosition = marker.GetMarkedCellPosition();
            if (markedCell == null)
                return;
            SetGoal(markedCell);
        }
    }
    public void SetGoal(Cell markedCell)
    {
        generator.ClearHelperMap();

        path = GetPath(markedCell, tryToAvoid);

        if (path == null)
        {
            squadPanel.UpdateText(generator.world.cells[position.x, position.y]);
            return;
        }

        goal = markedCell;
        squadPanel.UpdateText(goal, path);
        foreach (Vector2Int pos in path)
        {
            generator.helperMap.SetTile(new Vector3Int(pos.x, pos.y, 0), generator.routePointTile);
        }
    }

    public List<Vector2Int> GetPath(Cell markedCell, List<int> tryToAvoid)
    {
        var path = PathFinding.FindPath(generator.world.cells, generator.world.cells[position.x, position.y], markedCell, tryToAvoid);

        if (path == null)
        {
            List<int> newAvoids = new List<int>(tryToAvoid);

            for (int id = 6; id <= 9; id++)
            {
                if (newAvoids.Contains(id))
                {
                    newAvoids.Remove(id);
                    var newPath = GetPath(markedCell, newAvoids);
                    if (newPath != null)
                    {
                        return newPath;
                    }
                }
            }
        }
        return path;
    }
}
