using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System.IO;

public class Squad : MonoBehaviour, ITapItemHandler, ITapCardHandler
{
    //[HideInInspector]
    public SquadData data;

    public ItemsAndClassesInfo info;

    public GameObject cardPrefab;
    public GameObject itemPrefab;

    public Transform contentCardsPanel;
    GridLayoutGroup contentGroup;

    PlayerMove move;
    public TextMeshProUGUI goldText;
    public TextMeshProUGUI provisionText;

    public TextMeshProUGUI actionHint;

    public Image changePanelModeButton;
    public Sprite inventoryIcon;
    public Sprite cardsIcon;

    public bool inventoryMode = false;

    public SquadActionMode mode = SquadActionMode.Normal;

    Card selectedCard;
    Item selectedItem;

    public GameObject itemHint;
    public GameObject cardHint;

    EventController eventer;
    Shop shop;
    SaveSystem save;

    public bool isWaitingCardFromEvent = false;


    public void TapItem(Item item)
    {
        foreach (Transform child in itemHint.transform)
        {
            child.gameObject.SetActive(false);
        }

        if (mode == SquadActionMode.ItemSelection)
        {
            if(item is Hill)
            {
                ApplyHill(selectedCard, (Hill)item);
                return;
            }
            if (!info.classes[(int)(selectedCard.type)].isSuitable(item))
            {
                return;
            }

            EquipItem(selectedCard, item);
            TapChangePanelModeButton();
        }
        else
        {
            if (item.patternID == 12)
            {
                data.provision += 20;
                DisplayResourcesCount();
                data.inventory.Remove(item);
                actionHint.gameObject.SetActive(false);
                mode = SquadActionMode.Normal;
                inventoryMode = true;
                changePanelModeButton.sprite = cardsIcon;
                DisplayItems();
                if (shop.isSaleMode)
                    shop.TapChangeModeButton();
                return;
            }
            else if (item is Weapon || item is Armor || item is Hill)
            {
                selectedItem = item;
                mode = SquadActionMode.CardsSelection;
                SetupActionHint("Выберите, на ком использовать " + info.items[item.patternID].title);
                TapChangePanelModeButton(false);
            }
        }
    }

    public void TapCard(Card card)
    {
        foreach (Transform child in cardHint.transform)
        {
            child.gameObject.SetActive(false);
        }
        
        if (mode == SquadActionMode.CardsSelection)
        {
            if (isWaitingCardFromEvent)
            {
                mode = SquadActionMode.Normal;
                actionHint.gameObject.SetActive(false);
                changePanelModeButton.gameObject.SetActive(true);
                eventer.TapCardInSquad(card);
                isWaitingCardFromEvent = false;
                return;
            }
            if(selectedItem is Hill)
            {
                ApplyHill(card, (Hill)selectedItem);
                return;
            }
            if (!info.classes[(int)(card.type)].isSuitable(selectedItem))
            {
                return;
            }

            EquipItem(card, selectedItem);
            TapChangePanelModeButton();
        }
        else
        {
            selectedCard = card;
            mode = SquadActionMode.ItemSelection;
            SetupActionHint("Выберите предмет для использования или экипировки " + card.name);
            TapChangePanelModeButton(false);
        }
    }

    private void ApplyHill(Card taredCard, Hill hill)
    {
        int hilledHp = Random.Range(hill.minHilledHp, hill.maxHilledHp + 1);
        taredCard.GetTakingHillResult(hilledHp);
        data.inventory.Remove(hill);
        actionHint.gameObject.SetActive(false);
        mode = SquadActionMode.Normal;
        inventoryMode = false;
        changePanelModeButton.sprite = inventoryIcon;
        DisplaySquadCards();
    }
    private void EquipItem(Card targetCard, Item item)
    {
        if (item is Weapon)
        {
            Weapon oldWeapon = targetCard.weapon;
            targetCard.weapon = (Weapon)item;
            ReplaceItem(item, oldWeapon);
        }
        else if (item is Armor)
        {
            Armor oldArmor = targetCard.armor;
            targetCard.armor = (Armor)item;
            ReplaceItem(item, oldArmor);
        }

        if (mode == SquadActionMode.CardsSelection)
        {
            actionHint.gameObject.SetActive(false);
            mode = SquadActionMode.Normal;
            DisplaySquadCards();
        }
    }
    public void KeepLoot(LootData loot)
    {
        foreach(Item item in loot.items)
        {
            data.inventory.Add(item);
        }
        if (inventoryMode)
        {
            DisplayItems();
        }
    }
    public void SetupActionHint(string text)
    {
        actionHint.gameObject.SetActive(true);
        actionHint.text = text;
    }

    public void ReplaceItem(Item itemToReplace, Item replacement, bool onlyone = true)
    {
        for(int i = 0; i < data.inventory.Count; i++)
        {
            if(data.inventory[i] == itemToReplace)
            {
                data.inventory[i] = replacement;
                if (onlyone) { return; }
            }
        }
    }
    public int provision { get { return data.provision; } set { 
            data.provision = value;
            if (data.provision < 0) { 
                data.provision = 0;
                if (File.Exists(save.loadPath))
                    File.Delete(save.loadPath);
                eventer.AddEventToQueue(eventer.GetEventByID("no_provision"));
            }
            DisplayResourcesCount();
        } 
    }

    public void TapChangePanelModeButton(bool normalize = true)
    {
        if (normalize)
        {
            actionHint.gameObject.SetActive(false);
            mode = SquadActionMode.Normal;
        }
        inventoryMode = !inventoryMode;
        changePanelModeButton.sprite = inventoryMode ? cardsIcon : inventoryIcon;
        if (inventoryMode)
        {
            DisplayItems();
        }
        else
        {
            if (shop.isSaleMode)
            {
                shop.TapChangeModeButton();
            }
            DisplaySquadCards();
        }
    }
    public void UseProvision(int days)
    {
        provision -= days * data.cards.Count;
        HillCardInDay(1);
    }
    public void Initialize()
    {
        contentGroup = contentCardsPanel.GetComponent<GridLayoutGroup>();
        shop = GameObject.Find("Shop").GetComponent<Shop>();
        move = GameObject.Find("player").GetComponent<PlayerMove>();
        eventer = GameObject.Find("Eventer").GetComponent<EventController>();
        save = GameObject.Find("SaveSystem").GetComponent<SaveSystem>();
    }

    public SquadData CreateNewSquad()
    {
        List<Card> cards = new List<Card>() { new Card(1, info), new Card(Random.Range(3, 5), info), CreateRandomNewMercenary(info) };
        Vector2Int startPosition = GetNoWaterRandomPos(move.generator.world);
        return new SquadData(100, 80, new Vec2(startPosition), cards, new List<Item>());
    }
    public Vector2Int GetNoWaterRandomPos(WorldData world)
    {
        Vector2Int startPosition = new Vector2Int(Random.Range(0, world.cells.GetLength(0)), Random.Range(0, world.cells.GetLength(1)));
        if(world.cells[startPosition.x, startPosition.y].id == 0)
            return GetNoWaterRandomPos(world);
        return startPosition;
    }
    public void LoadSquad(SquadData squad)
    {
        data = squad;
        move.position = squad.position.Convert();
        move.goal = move.generator.world.cells[move.position.x, move.position.y];

        move.squadPanel.UpdateText(move.generator.world.cells[move.position.x, move.position.y], move.path);
        Camera.main.transform.position = new Vector3(move.position.x, move.position.y, Camera.main.transform.position.z);

        move.UpdatePositionDisplay();
        DisplaySquadCards();
        DisplayResourcesCount();
    }
    public void DisplayResourcesCount()
    {
        goldText.text = ": " + data.gold;
        provisionText.text = ": " + data.provision;
    }
    public void DisplaySquadCards()
    {
        contentGroup.cellSize = new Vector2(120, 173);
        ClearContent();
        foreach(Card card in data.cards)
        {
            AddCardToContent(card);
        }
    }

    public void DisplayItems()
    {
        contentGroup.cellSize = new Vector2(100, 100);
        ClearContent();
        foreach (Item item in data.inventory)
        {
            AddItemToContent(item);
        }
    }

    public void HillCardInDay(int days)
    {
        foreach(Card card in data.cards)
        {
            card.GetTakingHillResult(days);
            if (inventoryMode == false)
            DisplaySquadCards();
        }
    }
    public void AddCardToContent(Card card)
    {
        GameObject newCard = Instantiate(cardPrefab, contentCardsPanel);
        if((mode == SquadActionMode.CardsSelection) && !info.classes[(int)(card.type)].isSuitable(selectedItem))
        {
            newCard.GetComponent<Image>().color = new Color(1f, 1f, 1f, 0.5f);
        }
        SquadMemberCardPrefab slot = newCard.GetComponent<SquadMemberCardPrefab>();
        slot.SetType(ItemPrefabType.SquadItem);
        slot.SetDisplay(card);
    }
    public void AddItemToContent(Item item)
    {
        GameObject newItemPrefab = Instantiate(itemPrefab, contentCardsPanel);
        if ((mode == SquadActionMode.ItemSelection) && !info.classes[(int)(selectedCard.type)].isSuitable(item))
        {
            newItemPrefab.GetComponent<Image>().color = new Color(1f, 1f, 1f, 0.5f);
        }
        ItemPrefab slot = newItemPrefab.GetComponent<ItemPrefab>();
        slot.SetType(ItemPrefabType.SquadItem);
        slot.SetDisplay(item);
    }
    public void ClearContent()
    {
        foreach (Transform child in contentCardsPanel)
        {
            Destroy(child.gameObject);
        }
    }
    public static Card CreateRandomNewMercenary(ItemsAndClassesInfo info)
    {
        Class type = (Class)Random.Range(0, info.classes.Length);
        Card mercenary = new Card((int)type, info);
        return mercenary;
    }

    public static Card GetRandomEnemy(ItemsAndClassesInfo info)
    {
        Class type = (Class)Random.Range(info.classes.Length, info.classes.Length+info.enemyPatterns.Length);
        Card mercenary = new Card((int)type - info.classes.Length, info, true);
        return mercenary;
    }

}

[System.Serializable]
public class SquadData
{
    public int gold;
    public int provision;
    public Vec2 position;

    public List<Item> inventory;
    public List<Card> cards;

    public SquadData(int gold, int provision, Vec2 position, List<Card> cards, List<Item> inventory)
    {
        this.position = position;
        this.gold = gold;
        this.provision = provision;
        this.cards = cards;
        this.inventory = inventory;
    }
}

[System.Serializable]
public class Vec2
{
    public int x;
    public int y;
    public Vec2(Vector2Int vector)
    {
        x = vector.x;
        y = vector.y;
    }
    public Vector2Int Convert()
    {
        return new Vector2Int(x, y);
    }
}

public enum SquadActionMode
{
    Normal,
    ItemSelection,
    CardsSelection
}
[System.Serializable]
public class ClassInfo
{
    public string name;

    public Class mercenaryClass;

    public int minStartHp;
    public int maxStartHp;

    public int minStartBaseDamage;
    public int maxStartBaseDamage;

    public int minBaseDamageOffset;
    public int maxBaseDamageOffset;

    public int minBaseCriticalStrikeChance;
    public int maxBaseCriticalStrikeChance;

    public int minBaseProtection;
    public int maxBaseProtection;

    public int minBaseProtectionOffset;
    public int maxBaseProtectionOffset;

    public int minBaseEvasionChance;
    public int maxBaseEvasionChance;

    public int minPrice;
    public int maxPrice;

    public WeaponType[] sutableWeapons;
    public ArmorType[] sutableArmors;

    public WeaponType[] weaponsOnStart;
    public ArmorType[] armorOnStart;

    public Skill[] skills;

    public Sprite[] icnons;

    public bool isSuitable(Item item)
    {
        if(item is Weapon)
        {
            foreach(WeaponType type in sutableWeapons)
            {
                if(type == (item as Weapon).type)
                    return true;
            }
            return false;
        }
        else if (item is Armor)
        {
            foreach (ArmorType type in sutableArmors)
            {
                if (type == (item as Armor).type)
                    return true;
            }
            return false;
        }
        else if(item is Hill)
            return true;
        else
            return false;
    }
}