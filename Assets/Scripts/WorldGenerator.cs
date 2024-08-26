using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class WorldGenerator : MonoBehaviour
{
    public WorldData world;

    public Tilemap map;
    public Tilemap helperMap;

    public Tile routePointTile;
    public CellInfo[] cellInfo;

    MercenarySquadPanel squadPanel;

    public void Initialize()
    {
        squadPanel = GameObject.Find("squadPanel").GetComponent<MercenarySquadPanel>();
    }

    public void LoadWorld(WorldData world)
    {
        this.world = world;
        DisplayWorld(world);

        Vector2Int playerPos = GameObject.Find("player").GetComponent<PlayerMove>().position;
        Cell startCell = world.cells[playerPos.x, playerPos.y];
        GameObject.Find("player").GetComponent<PlayerMove>().goal = startCell;
        squadPanel.UpdateText(startCell);
    }
    public float GetDaysToPath(List<Vector2Int> path)
    {
        float result = 0;
        foreach(Vector2Int pos in path)
        {
            result += GetCellInfo(world.cells[pos.x, pos.y]).daysToMove;
        }
        return result;
    }
    public CellInfo GetCellInfo(Cell cell)
    {
        return cellInfo[cell.id];
    }

    public void ClearHelperMap()
    {
        int width = world.cells.GetLength(0);
        int height = world.cells.GetLength(1);
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                helperMap.SetTile(new Vector3Int(x, y, 0), null);
            }
        }
    }
    public void DisplayWorld(WorldData world)
    {
        int width = world.cells.GetLength(0);
        int height = world.cells.GetLength(1);
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                map.SetTile(new Vector3Int(x, y, 0), cellInfo[world.cells[x, y].id].tile);
            }
        }

    }
    public static WorldData GenerateWorld(int width, int height, ItemsAndClassesInfo info)
    {
        WorldData world = new WorldData(new Cell[width, height]);
        Vector2Int offset = new Vector2Int(Random.Range(-999*999, 999*999), Random.Range(-999 * 999, 999 * 999));
        float zoom = 5f;
        float intensity = 1f;

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                float nx = (x + offset.x) / zoom;
                float ny = (y + offset.y) / zoom;
                float n = Mathf.PerlinNoise(nx, ny) * intensity * Random.Range(0.8f, 1.2f); ;
                if (n > 0.28f)
                {
                    n *= Random.Range(0.7f, 1.3f);
                }
                world.cells[x, y] = GetCellIDByNoise(n, info);
            }
        }
        SpreadNumberAround(3, 2, world);
        SpreadNumberAround(2, 2, world);

        ReplaceSquadCells(world, 6, 9);

        PlaceVampireMansions(world, Random.Range(6, 9));

        for(int i =0; i < Random.Range(7,10); i++)
        ReplaceOneRandomCell(world, 7, 11);

        SetTownsSiegArmyPos(world);

        return world;
    }

    private static void SetTownsSiegArmyPos(WorldData world)
    {
        int width = world.cells.GetLength(0);
        int height = world.cells.GetLength(1);

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                if (world.cells[x, y] is Town)
                {
                    SetTownSiegArmyPos(world, ((Town)world.cells[x, y]), new Vector2Int(x, y));
                    world.towns.Add(((Town)world.cells[x, y]));
                }
            }
        }
    }
    private static void SetTownSiegArmyPos(WorldData world, Town town, Vector2Int townPos)
    {
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                Vector2Int localPos = new Vector2Int(townPos.x + x, townPos.y + y);
                if (localPos.x >= world.cells.GetLength(0) || localPos.y >= world.cells.GetLength(1) || localPos.x <= 0 || localPos.y <= 0)
                    continue;
               
                Cell cell = world.cells[localPos.x, localPos.y];
                if (cell.id == 1)
                {
                    town.siegArmyPos.Add(new Vec2(localPos));
                }
                
            }
        }
    }
    private static void PlaceVampireMansions(WorldData world, int numberOfMansions)
    {
        int width = world.cells.GetLength(0);
        int height = world.cells.GetLength(1);
        List<Vector2Int> possibleLocations = new List<Vector2Int>();

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                if (IsValidMansionLocation(x, y, world))
                {
                    possibleLocations.Add(new Vector2Int(x, y));
                }
            }
        }

        for (int i = 0; i < numberOfMansions && possibleLocations.Count > 0; i++)
        {
            int index = Random.Range(0, possibleLocations.Count);
            Vector2Int mansionLocation = possibleLocations[index];
            possibleLocations.RemoveAt(index);
            world.cells[mansionLocation.x, mansionLocation.y] = new Cell(10);
        }
    }

    private static bool IsValidMansionLocation(int x, int y, WorldData world)
    {
        int width = world.cells.GetLength(0);
        int height = world.cells.GetLength(1);

       
        bool nearMountainsOrThicket = false;
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                {
                    int cellID = world.cells[nx, ny].id;
                    if (cellID == 7 || cellID == 9)
                    {
                        nearMountainsOrThicket = true;
                    }
                }
            }
        }

        
        bool farFromVillage = true;
        for (int dx = -4; dx <= 4; dx++)
        {
            for (int dy = -4; dy <= 4; dy++)
            {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                {
                    if (world.cells[nx, ny].id == 3 || world.cells[nx, ny].id == 5)
                    {
                        farFromVillage = false;
                    }
                }
            }
        }

        return nearMountainsOrThicket && farFromVillage;
    }

    public static List<(int x, int y)> GetAllCellCoordinatesWithID(WorldData world, int id)
    {
        List<(int x, int y)> result = new List<(int x, int y)>();

        int width = world.cells.GetLength(0);
        int height = world.cells.GetLength(1);

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                if (world.cells[i, j].id == id)
                {
                    result.Add((i, j));
                }
            }
        }
        return result;
    }

    public static void ReplaceOneRandomCell(WorldData world, int a, int b)
    {
        int width = world.cells.GetLength(0);
        int height = world.cells.GetLength(1);
        Cell cell = world.cells[Random.Range(0, width), Random.Range(0, height)];

        if (cell.id == a)
        {
            cell.id = b;
            return;
        }
        else
        {
            ReplaceOneRandomCell(world, a, b);
        }
    }

    public static void ReplaceSquadCells(WorldData world, int a, int b)
    {
        int width = world.cells.GetLength(0);
        int height = world.cells.GetLength(1);

        for (int i = 0; i < width - 1; i++)
        {
            for (int j = 0; j < height - 1; j++)
            {
                // Проверяем, образуют ли 2x2 квадрат клетки с ID a
                if (world.cells[i, j].id == a &&
                    world.cells[i, j + 1].id == a &&
                    world.cells[i + 1, j].id == a &&
                    world.cells[i + 1, j + 1].id == a)
                {
                    // Заменяем ID на b
                    world.cells[i, j].id = b;
                    world.cells[i, j + 1].id = b;
                    world.cells[i + 1, j].id = b;
                    world.cells[i + 1, j + 1].id = b;
                }
            }
        }
    }
    public static Cell GetCellIDByNoise(float value, ItemsAndClassesInfo info)
    {
        Cell result = new Cell(0);
        if (value > 0.28f)
            result = new Cell(1);
        if(value > 0.455f && value < 0.466f)
        {
            var village = new Village(3, (VillageType)Random.Range(0, 4), Shop.GetVillageShop(info), Shop.GetTavernShop(info));
            var town = new Town(5, Shop.GetTownShop(info), Shop.GetTavernShop(info), Random.Range(1, 55));

            result = Random.Range(0, 6) == 0 ? town : village;
        }
        if (value > 0.6f)
            result = new Cell(6);
        if (value > 0.74f)
            result = new Cell(7);
        return result;
    }
    private static void SpreadNumberAround(int targetCellID, int spreadCellID, WorldData world, bool onlyOne = false)
    {
        for (int x = 0; x < world.cells.GetLength(0); x++)
        {
            for (int y = 0; y < world.cells.GetLength(1); y++)
            {
                if (world.cells[x, y].id == targetCellID && Random.Range(0, 2) == 0)
                {
                    TryToSpreadCell(x, y, spreadCellID, world);
                    if (onlyOne)
                        return;
                }
            }
        }
    }

    private static void TryToSpreadCell(int x, int y, int id, WorldData world)
    {
        int offsetX = Random.Range(-1, 2);
        int offsetY = Random.Range(-1, 2);

        if (offsetX == 0 && offsetY == 0)
        {
            TryToSpreadCell(x, y, id, world);
            return;
        }

        int newX = x + offsetX;
        int newY = y + offsetY;

        if (newX >= 0 && newX < world.cells.GetLength(0) && newY >= 0 && newY < world.cells.GetLength(1))
        {
                if (world.cells[newX, newY].id != 0)
                {
                    world.cells[newX, newY].id = id;
                }
        }
            
        
    }
}

[System.Serializable]
public class CellInfo
{
    public string title;
    public string baseDiscription;
    public float daysToMove;
    public int eventChance;
    public TileBase tile;

    public bool isExplainable = true;
}


[System.Serializable]
public class WorldData
{
    public Cell[,] cells;

    public List<Cell> newlyExploredCells = new();
    public List<ShopData> shopsToProvisionRestoration = new();
    public List<Town> towns = new();

    public int vampiresBeatCount = 0;

    public WorldData(Cell[,] cells)
    {
        this.cells = cells;
    }
}
[System.Serializable]
public class Cell
{
    public int id;
    public int daysBeforeUpdate = 0;
    public Cell(int id)
    {
        this.id = id;
    }
}

public interface IBuildingWithShop
{
    public ShopData shop { get; }
}
public interface IBuildingWithTavern
{
    public ShopData tavern { get; }
}
[System.Serializable]
public class SiegArmyCell : Cell
{
    public int priceForHelp;
    public Town town;

    public int fightDifficulty;
    public SiegArmyCell(int id, int priceForHelp, Town town, int fightDifficulty) : base(id)
    {
        this.priceForHelp = priceForHelp;
        this.town = town;
        this.fightDifficulty = fightDifficulty;
    }
}
[System.Serializable]
public class Village : Cell, IBuildingWithShop, IBuildingWithTavern
{
    public VillageType type;

    public ShopData shop { get; set; }
    public ShopData tavern { get; set; }
    public Village(int id, VillageType type, ShopData shop, ShopData tavern) : base(id)
    {
        this.tavern = tavern;
        this.type = type;
        this.shop = shop;
    }
}

[System.Serializable]
public class Town : Cell, IBuildingWithShop, IBuildingWithTavern
{
    public bool isSieged;
    public List<Vec2> siegArmyPos = new();
    public int daysToChangeStage;

    public int priceForHelp;
    public int fightDifficulty;

    public ShopData shop { get; set; }
    public ShopData tavern { get; set; }


    public Town(int id, ShopData shop, ShopData tavern, int daysToChangeStage) : base(id)
    {
        this.tavern = tavern;
        this.shop = shop;
        this.daysToChangeStage = daysToChangeStage;
    }

    public void StartSiege(WorldData world)
    {
        if(siegArmyPos == null)
            return;
        isSieged = true;

        int difficultySieg = Random.Range(1, 4);
        fightDifficulty = Random.Range(1, 4);

        priceForHelp = Random.Range(30, 40) + fightDifficulty*60;
        int atackPrice = Random.Range(20, 30) + difficultySieg*60;

        foreach (Vec2 vec in siegArmyPos)
            world.cells[vec.x, vec.y] = new SiegArmyCell(4, atackPrice, this, difficultySieg);
        daysToChangeStage = Random.Range(8, 18);
    }
    public void EndSiege(WorldData world)
    {
        if (siegArmyPos == null)
            return;
        isSieged = false;
        foreach (Vec2 vec in siegArmyPos)
            world.cells[vec.x, vec.y] = new Cell(1);
        daysToChangeStage = Random.Range(20, 40);

    }
}


[System.Serializable]

public enum VillageType
{
    Small,
    Medium,
    Big,
    Large
}