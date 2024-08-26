using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using UnityEngine;

public class SaveSystem : MonoBehaviour
{
    WorldGenerator generator;
    Squad squad;
    DayTimer timer;
    EventController eventer;
    Shop shop;
    Loot loot;
    ScenChanger scenChanger;
    PlayerMove move;

    public string loadPath;

    SaveData save;
    SettingsData settings;

    string settingsPath;

    private void Start()
    {
        generator = GameObject.Find("generator").GetComponent<WorldGenerator>();
        squad = GameObject.Find("squad").GetComponent<Squad>();
        timer = GameObject.Find("timer").GetComponent<DayTimer>();
        eventer = GameObject.Find("Eventer").GetComponent<EventController>();
        loadPath = PlayerPrefs.GetString("loadPath");
        shop = GameObject.Find("Shop").GetComponent<Shop>();
        loot = GameObject.Find("Loot").GetComponent<Loot>();
        scenChanger = GameObject.Find("Canvas").GetComponent<ScenChanger>();
        move = GameObject.Find("player").GetComponent<PlayerMove>();

        settingsPath = PlayerPrefs.GetString("savePath") + @"\settings.settings";

        Load();
    }
    public void Save()
    {
        string directoryPath = Path.GetDirectoryName(loadPath);

        if (!Directory.Exists(directoryPath))
            Directory.CreateDirectory(directoryPath);

        BinaryFormatter formatter = new BinaryFormatter();

        WorldData world = generator.world;

        world.newlyExploredCells = timer.newlyExploredCells;
        world.shopsToProvisionRestoration = timer.shopsToProvisionRestoration;

        SquadData squadData = squad.data;
        FightData fightData = eventer.fight;
        LootData lootData = loot.data;
        System.DateTime currentDateTime = System.DateTime.Now;
        string formattedTime = currentDateTime.ToString("dd.MM.yyyy HH:mm");


        SaveData data = new SaveData(world, squadData, save.name, save.creationTime, formattedTime, timer.GetDays());
        data.currentFight = fightData;
        data.currentLoot = lootData;
        data.currentEvent = eventer.eventGoingNow ? eventer.currentEvent : null;

        using (FileStream fs = new FileStream(loadPath, FileMode.OpenOrCreate))
        {
            formatter.Serialize(fs, data);
        }
    }
    public void Load()
    {
        BinaryFormatter formatter = new BinaryFormatter();

        using (FileStream fs = new FileStream(settingsPath, FileMode.OpenOrCreate))
        {
            SettingsData settings = (SettingsData)formatter.Deserialize(fs);
            this.settings = settings;
            GameObject.Find("SoundManager").GetComponent<LerpSound>().LerpTo(settings.soundVolume / 100f);
            GameObject.Find("Main Camera").GetComponent<LerpSound>().LerpTo(settings.musicVolume / 100f);
            if (settings.isAvoidgMountains)
            {
                move.tryToAvoid.Add(7);
                move.tryToAvoid.Add(8);
            }
            if (settings.isAvoidgForests)
            {
                move.tryToAvoid.Add(6);
                move.tryToAvoid.Add(9);
            }
        }

        using (FileStream fs = new FileStream(loadPath, FileMode.OpenOrCreate))
        {
            if (fs.Length <= 0)
            {
                fs.Close();
                File.Delete(loadPath);
                return;
            }
            SaveData data = (SaveData)formatter.Deserialize(fs);
            save = data;
            generator.Initialize();
            generator.LoadWorld(data.world);

            timer.newlyExploredCells = data.world.newlyExploredCells;
            timer.shopsToProvisionRestoration = data.world.shopsToProvisionRestoration;
            timer.towns = data.world.towns;

            
            squad.Initialize();
            if (data.squad == null)
            {
                data.squad = squad.CreateNewSquad();
            }
            squad.LoadSquad(data.squad);

            timer.SetDays(data.days);
            shop.Initialize();
            if(data.currentFight != null && data.currentFight.playerCards.Length > 0)
            {
                scenChanger.StartOpenScen("Fight");
            }
            if (data.currentEvent != null)
            {
                eventer.AddEventToQueue(data.currentEvent);
            }
            loot.data = data.currentLoot;
            if(loot.data != null)
            {
                loot.ShowLoot();
            }

        }
    }
}


[System.Serializable]
public class SaveData
{
    public WorldData world;
    public SquadData squad;

    public FightData currentFight;

    public LootData currentLoot;

    public Event currentEvent;

    public string name;
    public string creationTime;
    public string lastLaunchTime;

    public int days;
    public SaveData(WorldData world, SquadData squad, string name, string creationTime, string lastLaunchTime, int days)
    {
        this.world = world;
        this.squad = squad;
        this.name = name;
        this.creationTime = creationTime;
        this.lastLaunchTime = lastLaunchTime;
        this.days = days;
    }
}