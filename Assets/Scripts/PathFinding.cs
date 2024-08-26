using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class PathFinding 
{
    public static List<Vector2Int> FindPath(Cell[,] grid, Cell start, Cell end, List<int> avoidingID)
    {
        if (avoidingID.Contains(end.id))
        {
            return null;
        }
        var queue = new Queue<Cell>();
        var visited = new HashSet<Vector2Int>();
        var path = new Dictionary<Vector2Int, Vector2Int>();
        var directions = new List<Vector2Int>
        {
            new Vector2Int(0, 1), 
            new Vector2Int(1, 0),  
            new Vector2Int(0, -1), 
            new Vector2Int(-1, 0)
        };

        queue.Enqueue(start);
        visited.Add(GetCellPosition(grid, start));

        while (queue.Count > 0)
        {
            var current = queue.Dequeue();
            if (GetCellPosition(grid, current) == GetCellPosition(grid, end))
            {
                return ReconstructPath(path, GetCellPosition(grid, start), GetCellPosition(grid, end));
            }

            foreach (var dir in directions)
            {
                Vector2Int neighbourPos = GetCellPosition(grid, current) + dir;
                if (neighbourPos.x >= 0 && neighbourPos.x < grid.GetLength(0) &&
                    neighbourPos.y >= 0 && neighbourPos.y < grid.GetLength(1) &&
                    !avoidingID.Contains(grid[neighbourPos.x, neighbourPos.y].id) &&
                    !visited.Contains(neighbourPos))
                {
                    queue.Enqueue(grid[neighbourPos.x, neighbourPos.y]);
                    visited.Add(neighbourPos);
                    path[neighbourPos] = GetCellPosition(grid, current);
                }
            }
        }

        return null; 
    }

    public static Vector2Int GetCellPosition (Cell[,] cells, Cell cell)
    {
        for (int i = 0; i < cells.GetLength(0); i++)
        {
            for (int j = 0; j < cells.GetLength(1); j++)
            {
                if (cells[i, j].Equals(cell))
                {
                    return new Vector2Int(i, j);
                }
            }
        }
        return new Vector2Int(0, 0); 
    }

    private static List<Vector2Int> ReconstructPath(Dictionary<Vector2Int, Vector2Int> path, Vector2Int start, Vector2Int end)
    {
        var fullPath = new List<Vector2Int>();
        var current = end;
        while (current != start)
        {
            fullPath.Add(current);
            current = path[current];
        }
        fullPath.Add(start); 
        fullPath.Reverse();

        fullPath.Remove(fullPath[0]);
        return fullPath;
    }
}
