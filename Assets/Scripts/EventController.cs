using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using TMPro;
using System.Linq;

public class EventController : MonoBehaviour
{
    public List<Event> events;
    public List<Replacement> replacements = new();


    public Animator squadPanel;

    public GameObject squadInfo;
    public GameObject eventInfo;

    public TextMeshProUGUI eventTitleText;
    public TextMeshProUGUI eventText;

    public GameObject[] eventButtons;

    public List<Event> queueToShow;
    public Event currentEvent;
    public bool eventGoingNow = false;
    int eventTextIndex = 0;
    float timer;
    float time = 0.03f;

    public PlayerMove move;
    public Shop shop;
    public WorldGenerator generator;
    public DayTimer dayTimer;
    public MercenarySquadPanel squadInfoPanel;
    public Loot loot;
    public ScenChanger sceneChanger;

    SaveSystem saveSys;
    Squad squad;

    [HideInInspector]
    public FightData fight;


    public ItemsAndClassesInfo info;

    public ScenChanger scenChanger;
    AudioSource sounder;

    public AudioClip pub;

    public Event GetEventByID(string id)
    {
        foreach(Event e in events)
        {
            if(e.eventId == id)
                return e;
        }
        return events[0];
    }
    public void AddEventToQueue(Event gameEvent)
    {
        queueToShow.Add(gameEvent);
        if(queueToShow.Count == 1)
            ShowNextEvent();
    }
    public void HideButtons()
    {
        for (int i = 0; i < eventButtons.Length; i++)
        {
            eventButtons[i].SetActive(false);
        }
    }
    public void DisplayButtons()
    {
        for(int i = 0; i < currentEvent.answers.Count; i++)
        {
            eventButtons[i].transform.GetChild(0).gameObject.GetComponent<TextMeshProUGUI>().text = currentEvent.answers[i].answerText;
            eventButtons[i].SetActive(true);
        }
    }
    public void ShowNextEvent()
    {
        eventGoingNow = true;
        HideButtons();
        squadPanel.SetBool("collaps", false);
        currentEvent = queueToShow[0];
        squadInfo.SetActive(false);
        eventInfo.SetActive(true);
        eventTitleText.text = currentEvent.eventName;
        eventTextIndex = 0;
        string text = currentEvent.eventText;
        foreach(Replacement replacement in replacements)
            text = text.Replace(replacement.input, replacement.output);
        eventText.text = text;

        eventText.maxVisibleCharacters = eventTextIndex;

        if(currentEvent.eventId == "village_base" || currentEvent.eventId == "town_base" || currentEvent.eventId == "village_tavern")
        {
            sounder.PlayOneShot(pub);
        }

    }
    public List<Event> ExplainCellEvent(Cell cell)
    {
        List<Event> result = new List<Event>();
        foreach(Event e in events)
        {
            if (cell.id == e.eventTypeId && e.isOnlyPass == false)
                result.Add(e);
        }
        return result;
    }
    public Event GetRandomEventByType(int eventTypeId, List<Event> events)
    {
        var filteredEvents = events.Where(e => e.eventTypeId == eventTypeId).ToList();
        int totalWeight = filteredEvents.Sum(e => e.weight);

        int randomValue = Random.Range(0, totalWeight);
        int currentWeightSum = 0;

        foreach (var e in filteredEvents)
        {
            currentWeightSum += e.weight;
            if (randomValue < currentWeightSum)
                return e;
        }

        return null; // на случай, если что-то пошло не так
    }
    public List<Event> PassingCellEvent(Cell cell)
    {
        List<Event> result = new List<Event>();
        foreach (Event e in events)
        {
            if (cell.id == e.eventTypeId && !e.isOnlyExplain)
                result.Add(e);
        }
        return result;
    }

    public void ExlplainCell(Cell cell)
    {
        if(cell.daysBeforeUpdate > 0)
        {
            AddEventToQueue(GetEventByID("explained"));
            return;
        }
        if (generator.cellInfo[cell.id].isExplainable) 
        {
            cell.daysBeforeUpdate = 30;
            dayTimer.newlyExploredCells.Add(cell);
            squadInfoPanel.UpdateText(cell);
        }
       
        if(cell is Town && ((Town)cell).isSieged)
        {
            replacements = new List<Replacement> { new Replacement("%gold%", ((Town)cell).priceForHelp.ToString()) };
            AddEventToQueue(GetEventByID("town_sieged"));
            return;
        }
        else if(cell is SiegArmyCell)
            replacements = new List<Replacement> { new Replacement("%gold%", ((SiegArmyCell)cell).priceForHelp.ToString()) };
        
        List<Event> events = ExplainCellEvent(cell);
        if(events.Count == 0)
            return;
        Event randEvent = GetRandomEventByType(cell.id, events);
        AddEventToQueue(randEvent);
    }
    public void PassingCell(Cell cell)
    {
        if (cell.daysBeforeUpdate > 0 || (Random.Range(0, 101) > generator.cellInfo[cell.id].eventChance))
            return;
        List<Event> events = PassingCellEvent(cell);
        if (events.Count == 0)
            return;
        Event randEvent = GetRandomEventByType(cell.id, events);
        AddEventToQueue(randEvent);
    }

    public void EndEvent()
    {
        eventTextIndex = 0;
        eventGoingNow = false;
        HideButtons();
        squadPanel.SetBool("collaps", true);
        queueToShow.Remove(currentEvent);
        currentEvent = null;
        eventTitleText.text = "";
        squadInfo.SetActive(true);
        eventInfo.SetActive(false);
        eventText.text = "";
        eventText.maxVisibleCharacters = eventTextIndex;

        if(queueToShow.Count > 0)
            ShowNextEvent();
    }

    public void TapEventButton(int id)
    {
        string nextEventID = currentEvent.answers[id].nextEventId;
        if (nextEventID != "none")
            AddEventToQueue(GetEventByID(nextEventID));
        
        Action(currentEvent.answers[id].actionId);
        EndEvent();
    }
    public void Action(string id)
    {
        Cell cell = generator.world.cells[move.position.x, move.position.y];

        Card unit;

        switch (id)
        {
            case "add_provision":
                squad.provision += Random.Range(3, 9);
                squad.DisplayResourcesCount();
                break;
            case "forest_animal_fight":
                List<Card> enemyes = new();
                int cardID = Random.Range(0, 2);
                for(int i = 0; i < (cardID == 0 ? Random.Range(2, 5) : Random.Range(4, 7)); i++)
                    enemyes.Add(new Card(cardID, info, true));
                if(cardID == 0)
                {
                    List<Item> bearSkins = new();
                    for(int i = 0; i < enemyes.Count; i++)
                        bearSkins.Add(new Item(13, info.items[13].cost));
                    loot.data = new LootData("Добыча с медведей", bearSkins);
                }
                StartFight(enemyes);
                break;
            case "hard_forest_animal_fight":
                enemyes = new();
                cardID = Random.Range(0, 2);
                for (int i = 0; i < (cardID == 0 ? Random.Range(5, 9) : Random.Range(7, 11)); i++)
                    enemyes.Add(new Card(cardID, info, true, Random.Range(2, 5)));
                if (cardID == 0)
                {
                    List<Item> bearSkins = new();
                    for (int i = 0; i < enemyes.Count; i++)
                        bearSkins.Add(new Item(13, info.items[13].cost));
                    loot.data = new LootData("Добыча с медведей", bearSkins);
                }
                StartFight(enemyes);
                break;
            case "harpia_fight":
                List<Card> harpies = new();

                for (int i = 0; i < Random.Range(1, 3); i++)
                    harpies.Add(new Card(6, info, true));
                StartFight(harpies);
                break;
            case "try_outlaws_give_gold":
                if(squad.data.gold >= 20)
                {
                    squad.data.gold -= 20;
                    squad.DisplayResourcesCount();
                    return;
                }
                Action("outlaws_fight");
                break;
            case "outlaws_fight":
                List<Card> outlawsCamp = new();
                Card leader = new Card(3, info, true, Random.Range(1, 3));
                outlawsCamp.Add(leader);
                for (int i = 0; i < Random.Range(1, 4); i++)
                {
                    unit = new Card(4, info, true, Random.Range(1, 3));
                    unit.leader = leader;
                    outlawsCamp.Add(unit);
                }
                for (int i = 0; i < Random.Range(1, 4); i++)
                {
                    unit = new Card(5, info, true, Random.Range(1, 3));
                    unit.leader = leader;
                    outlawsCamp.Add(unit);
                }
                StartFight(outlawsCamp);
                break;

            case "outlaws_camp":
                outlawsCamp = new();
                leader = new Card(3, info, true, Random.Range(1, 3));
                outlawsCamp.Add(leader);
                for (int i = 0; i < 4; i++)
                {
                    unit = new Card(4, info, true, Random.Range(1, 3));
                    unit.leader = leader;
                    outlawsCamp.Add(unit);
                }
                for (int i = 0; i < 5; i++)
                {
                    unit = new Card(5, info, true, Random.Range(1, 3));
                    unit.leader = leader;
                    outlawsCamp.Add(unit);
                }
                loot.data = Loot.GetBanditCampLoot(info);
                StartFight(outlawsCamp);
                break;
            case "show_village_shop":
                ShopData shopData = ((IBuildingWithShop)cell).shop;
                shop.Show(shopData);
                break;
            case "show_taverna_shop":
                ShopData tavern = ((IBuildingWithTavern)cell).tavern;
                shop.Show(tavern);
                break;
            case "close_shop":
                shop.Close();
                break;
            case "take_loot":
                squad.KeepLoot(loot.data);
                loot.ClosePanel();
                break;
            case "try_tavern_chill":
                int chill_gold = 0;
                foreach (Card card in squad.data.cards)
                {
                    if (card.hp != card.maxHp+card.addingMaxHp)
                        chill_gold += 5;
                }

                if(chill_gold == 0)
                    AddEventToQueue(GetEventByID("all_health"));
                else if (squad.data.gold >= chill_gold)
                {
                    squad.data.gold -= chill_gold;
                    shop.Show(shop.data);
                    squad.DisplayResourcesCount();
                    foreach (Card card in squad.data.cards)
                        card.hp = card.maxHp + card.addingMaxHp;
                    if(squad.inventoryMode == false)
                        squad.DisplaySquadCards();
                    
                    AddEventToQueue(GetEventByID("tavern_chill"));
                }
                else
                    AddEventToQueue(GetEventByID("tavern_need_gold"));
                
                break;
            case "to_menu":
                scenChanger.StartOpenScen("Menu");
                break;
            case "forest_search_camp":
                if(Random.Range(0, 2) == 0)
                    AddEventToQueue(GetEventByID("forest_fire2"));
                else
                {
                    squad.data.provision += Random.Range(10, 20);
                    loot.data = Loot.GetBanditCampLoot(info);
                    loot.ShowLoot();
                    AddEventToQueue(GetEventByID("forest_fire1"));
                    squad.DisplayResourcesCount();
                }
                break;
            case "clad":
                loot.data = Loot.GetBanditCampLoot(info);
                loot.ShowLoot();
                break;
            case "fire_spirit_fight":
                StartFight(new List<Card> { new Card(7, info, true) });
                break;
            case "listen_to_whispers":
                if (Random.Range(0, 2) == 0)
                {
                    for(int i = 0; i < squad.data.cards.Count / 2; i++)
                    {
                        squad.data.cards[i].ApplyEffect(new Effect(4, EffectTypeByDuration.ExpiringDays, 25), info);
                    }
                    if(squad.inventoryMode == false)
                        squad.DisplaySquadCards();
                    
                    AddEventToQueue(GetEventByID("whispers1"));
                }
                else
                {
                    squad.data.gold += 70;
                    squad.DisplayResourcesCount();
                    AddEventToQueue(GetEventByID("whispers2"));
                }
                break;
            case "lich_fight":
                loot.data = Loot.GetVampireLoot(info);
                List<Card> lich = new();
                lich.Add(new Card(14, info, true));
                generator.world.vampiresBeatCount++;
                AddEventToQueue(GetEventByID("won"));
                StartFight(lich, 1);
                break;
            case "vampires_fight":
                if(generator.world.vampiresBeatCount == 2)
                {
                    AddEventToQueue(GetEventByID("lich"));
                    break;
                }
                loot.data = Loot.GetVampireLoot(info);
                List<Card> vampires = new();

                for(int i = 0; i < Random.Range(1, 3); i++)
                    vampires.Add(new Card(9, info, true));
                generator.world.vampiresBeatCount++;
                StartFight(vampires);
                break;
            case "weapon_upgrade":
                squad.mode = SquadActionMode.CardsSelection;
                squad.SetupActionHint("Выберите кому кузнец подкуёт оружие");
                if(squad.inventoryMode)
                squad.TapChangePanelModeButton(false);
                squad.isWaitingCardFromEvent = true;
                squad.changePanelModeButton.gameObject.SetActive(false);
                break;
            case "troll_fight":
                loot.data = Loot.GetCaveLoot(info);
                enemyes = new();
                for (int i = 0; i < Random.Range(3, 5); i++)
                    enemyes.Add(new Card(12, info, true));
                StartFight(enemyes);
                break;
            case "skeleton_fight":
                loot.data = Loot.GetCaveLoot(info);
                enemyes = new();
                for (int i = 0; i < Random.Range(5, 8); i++)
                    enemyes.Add(new Card(11, info, true));
                StartFight(enemyes);
                break;
            case "cave_clad":
                if(Random.Range(0, 5) == 0)
                {
                    foreach(Card card in squad.data.cards)
                    {
                        card.hp -= Random.Range(5, 9);
                        if(card.hp <= 0)
                        {
                            if (squad.data.cards.Count > 1)
                                squad.data.cards.Remove(card);
                            else
                                card.hp = 1;
                        }
                    }
                    if (squad.inventoryMode)
                    {
                        squad.DisplaySquadCards();
                    }
                    AddEventToQueue(GetEventByID("cave_trap"));
                }
                else
                {
                    loot.data = Loot.GetCaveLoot(info);
                    loot.ShowLoot();
                }
                break;

            case "protect_town_fight":
                List<Card> townAtackers = new();
                int lvl = ((Town)cell).fightDifficulty;

                int lineCount = lvl == 1 ? Random.Range(4, 6) : 5;
                for (int i = 0; i < lineCount; i++)
                {
                    Card card = new Card(Random.Range(0, 3), info);
                    card.name = "Захватчик города";
                    if (lvl != 1)
                    {
                        for (int q = 0; q < Random.Range(0, lvl + 1); q++)
                            card.LevelUp();
                    }
                    townAtackers.Add(card);
                }
                lineCount = lvl == 1 ? Random.Range(4, 6) : 5;
                for (int i = 0; i < lineCount; i++)
                {
                    Card card = new Card(Random.Range(3, 5), info);
                    card.name = "Захватчик города";
                    if (lvl != 1)
                    {
                        for (int q = 0; q < Random.Range(0, lvl + 1); q++)
                            card.LevelUp();
                    }
                    townAtackers.Add(card);
                }

                StartFight(townAtackers);
                break;
            case "atack_town_fight":
                List<Card> townProtectors = new();
                lvl = ((SiegArmyCell)cell).fightDifficulty;

                lineCount = lvl == 1 ? Random.Range(4, 6) : 5;
                for (int i = 0; i < lineCount; i++)
                {
                    Card card = new Card(Random.Range(0, 3), info);
                    card.name = "Защитник города";
                    if (lvl != 1)
                    {
                        for (int q = 0; q < Random.Range(0, lvl + 1); q++)
                            card.LevelUp();
                    }
                    townProtectors.Add(card);
                }
                lineCount = lvl == 1 ? Random.Range(4, 6) : 5;
                for (int i = 0; i < lineCount; i++)
                {
                    Card card = new Card(Random.Range(3, 5), info);
                    card.name = "Защитник города";
                    if (lvl != 1)
                    {
                        for (int q = 0; q < Random.Range(0, lvl + 1); q++)
                            card.LevelUp();
                    }
                    townProtectors.Add(card);
                }

                StartFight(townProtectors);
                break;

            case "town_atacked_win":
                squad.data.gold += ((SiegArmyCell)cell).priceForHelp;
                ((SiegArmyCell)cell).town.EndSiege(generator.world);
                generator.DisplayWorld(generator.world);
                squad.DisplayResourcesCount();
                squadInfoPanel.UpdateText(cell);
                break;
            case "town_protected_win":
                squad.data.gold += ((Town)cell).priceForHelp;
                ((Town)cell).EndSiege(generator.world);
                generator.DisplayWorld(generator.world);
                squad.DisplayResourcesCount();
                squadInfoPanel.UpdateText(cell);
                break;
            case "try_still_eggs":
                loot.data = Loot.GetGriffinNestLoot(info);
                if(Random.Range(0, 3) == 0)
                {
                    loot.ShowLoot();
                    return;
                }

                List<Card> griffins = new();
                for (int i = 0; i < Random.Range(2, 5); i++)
                {
                    griffins.Add(new Card(13, info, true, Random.Range(1, 3)));
                }
                StartFight(griffins);
                break;
            case "hard_spirit_fight":
                List<Card> spirits = new();
                for(int i = 0; i < 3; i++)
                    spirits.Add(new Card(7, info, true, Random.Range(1, 3)));
                StartFight(spirits);
                break;
            case "crazy_swordman_fight":
                loot.data = Loot.GetSwordmanLoot(info);
                StartFight(new List<Card> { new Card(1, info, false, Random.Range(6, 8))});
                break;
        }
    }
    public void TapCardInSquad(Card card)
    {
        if(queueToShow[0].eventId == "weapon_upgrade_choose")
        {
            card.weapon.damage += 3;
            card.weapon.criticalStrikeChance += 1;
        }
        EndEvent();
    }
    public void StartFight(List<Card> enemyes, int musicID = 0)
    {
        List<Card> playerCards = squad.data.cards;
        fight = new FightData(playerCards, enemyes, musicID);
        EndEvent();
        saveSys.Save();
        scenChanger.StartOpenScen("Fight");
    }

    public void SkipTextAnimation()
    {
        if (eventGoingNow == false)
            return;
        eventTextIndex = currentEvent.eventText.Length;
        eventText.maxVisibleCharacters = eventTextIndex;
        DisplayButtons();
    }
    public void Update()
    {
        if(eventGoingNow == false)
            return;

        if(timer <= 0 && eventTextIndex < currentEvent.eventText.Length)
        {
            timer = time;
            eventTextIndex += 2;
            eventText.maxVisibleCharacters = eventTextIndex;
            if(eventTextIndex >= currentEvent.eventText.Length)
                DisplayButtons();

        }
        else
            timer -= Time.deltaTime;
    }

    void Start()
    {
        saveSys = GameObject.Find("SaveSystem").GetComponent<SaveSystem>();
        squad = GameObject.Find("squad").GetComponent<Squad>();
        sounder = GameObject.Find("SoundManager").GetComponent<AudioSource>();

        LoadEventsFromFile("events");


        //AddEventToQueue(GetEventByID(4));
    }

    void LoadEventsFromFile(string filePath)
    {
        TextAsset jsonFile = Resources.Load<TextAsset>(filePath);
        string jsonContent = jsonFile.text;
        EventList eventList = JsonUtility.FromJson<EventList>(jsonContent);
        events = eventList.events;
    }
}

[System.Serializable]
public class Replacement
{
    public string input;
    public string output;

    public Replacement(string input, string output)
    {
        this.input = input;
        this.output = output;
    }
}

[System.Serializable]
public class EventList
{
    public List<Event> events;
}
[System.Serializable]
public class Event
{
    public string eventName;
    public string eventText;
    public string eventId;
    public int eventTypeId;
    public bool isOnlyExplain;
    public bool isOnlyPass;
    public int weight;
    public List<Answer> answers;

    
}

[System.Serializable]
public class Answer
{
    public string answerText;
    public string nextEventId;
    public string actionId;
}