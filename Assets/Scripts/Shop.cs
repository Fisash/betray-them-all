using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class Shop : MonoBehaviour, ITapItemHandler, ITapCardHandler
{
    public ShopData data;

    public GameObject shopPanel;

    public Transform content;
    GridLayoutGroup grid;

    public ItemHintPanel itemPanel;
    public GameObject itemPrefab;

    public SquadMemberInfoPanel cardPanel;
    public GameObject cardPrefab;

    public ItemsAndClassesInfo info;
    public Squad squad;
    public DayTimer timer;

    public TextMeshProUGUI shopMoney;
    public TextMeshProUGUI changeModeButtonText;
    public TextMeshProUGUI hint;

    public TextMeshProUGUI shopName;

    public bool isSaleMode;

    public Transform playerItemsContent;

    public GameObject itemHint;
    AudioSource sounder;

    public AudioClip[] money;
    public void Initialize()
    {
        grid = content.GetComponent<GridLayoutGroup>();
        sounder = GameObject.Find("SoundManager").GetComponent<AudioSource>();
        /*List<ISaleable> cards = new List<ISaleable>() { Squad.CreateRandomNewMercenary(info), Squad.CreateRandomNewMercenary(info) };
        ShopData testData = new ShopData(cards, 200, ShopType.CardsShop, 1.2f, ShopName.Tavern);
        Show(testData);*/
    }
    public void Close()
    {
        if (isSaleMode)
        {
            TapChangeModeButton();
        }
        itemHint.SetActive(false);
        shopPanel.SetActive(false);
    }
    public void TapChangeModeButton()
    {
        itemHint.SetActive(false);
        isSaleMode = !isSaleMode;
        changeModeButtonText.text = isSaleMode ? "Купить предметы" : "Продать предметы";
        hint.text = isSaleMode ? "-Нажмите на предмет чтобы продать его-" : "-Нажмите на предмет чтобы купить его-";
        Show(data);
    }
    public void Show(ShopData data)
    {
        itemHint.SetActive(false);
        this.data = data;
        shopMoney.gameObject.SetActive(false);
        if (data.type == ShopType.ItemsShop)
        {
            //shopMoney.text = "Золото продавца: " + data.money.ToString();
            //shopMoney.gameObject.SetActive(true);
            changeModeButtonText.transform.parent.gameObject.SetActive(true);
            if (isSaleMode)
            {
                hint.text = "-Нажмите предмет чтобы продать его-";
                ShowItemsForSale();
            }
            else
            {
                hint.text = "-Нажмите предмет чтобы купить его-";
                ShowItemsForPurchase();
            }
        }
        else if(data.type == ShopType.CardsShop)
        {
            //shopMoney.gameObject.SetActive(false);
            changeModeButtonText.transform.parent.gameObject.SetActive(false);
            hint.text = "-Нажмите на наёмника чтобы купить его-";
            ShowCardsForPurchase();
        }
        shopName.text = data.GetName();
        shopPanel.SetActive(true);
    }

    public void ShowItemsForPurchase()
    {
        grid.cellSize = new Vector2(120, 120);
        ClearContent();
        foreach (Item item in data.goods)
        {
            GameObject newItemPrefab = Instantiate(itemPrefab, content);
            if (squad.data.gold < Mathf.RoundToInt(item.Price * data.priceDistortion))
            {
                newItemPrefab.GetComponent<Image>().color = new Color(1f, 1f, 1f, 0.5f);
            }
            ItemPrefab slot = newItemPrefab.GetComponent<ItemPrefab>();
            itemPanel.priceDistortion = data.priceDistortion;
            slot.SetType(ItemPrefabType.ShopItem);
            slot.panelName = "shopItemHint";
            slot.SetDisplay(item);
        }
    }

    public void ShowItemsForSale()
    {
        grid.cellSize = new Vector2(120, 120);
        ClearContent();
        foreach (Item item in squad.data.inventory)
        {
            GameObject newItemPrefab = Instantiate(itemPrefab, content);
            ItemPrefab slot = newItemPrefab.GetComponent<ItemPrefab>();
            itemPanel.priceDistortion = 1f;
            slot.SetType(ItemPrefabType.ShopItem);
            slot.panelName = "shopItemHint";
            slot.SetDisplay(item);
        }
    }
    public void ShowCardsForPurchase()
    {
        grid.cellSize = new Vector2(120, 170);
        ClearContent();
        foreach (Card card in data.goods)
        {
            GameObject newCardPrefab = Instantiate(cardPrefab, content);
            if (squad.data.gold < Mathf.RoundToInt(card.Price * data.priceDistortion) || squad.data.cards.Count >= 10)
            {
                newCardPrefab.GetComponent<Image>().color = new Color(1f, 1f, 1f, 0.5f);
            }
            SquadMemberCardPrefab prefab = newCardPrefab.GetComponent<SquadMemberCardPrefab>();
            prefab.panelName = "shopCardHint";
            cardPanel.priceDistortion = data.priceDistortion;
            prefab.SetType(ItemPrefabType.ShopItem);
            prefab.SetDisplay(card);
        }
    }
    public void TapItem(Item item) => TapGood(item);
    public void TapCard(Card card) => TapGood(card);
    
    public void TapGood(ISaleable saleable)
    {
        float priceDistortion = isSaleMode ? 1f : data.priceDistortion;
        int cost = Mathf.RoundToInt(saleable.Price * priceDistortion);

        if (isSaleMode && saleable is Item)
        {
            sounder.PlayOneShot(money[Random.Range(0, money.Length)]);
            squad.data.gold += cost;
            data.money -= cost;
            squad.data.inventory.Remove((Item)saleable);
            //data.goods.Add(saleable);
            Show(data);
            OpenInventoryInSquad();
            return;
        }

        if (squad.data.gold >= cost)
        {
            if(saleable is Item)
            {
                squad.data.gold -= cost;
                sounder.PlayOneShot(money[Random.Range(0, money.Length)]);
                data.money += cost;
                if (((Item)saleable).patternID == 12)
                {
                    if(timer.shopsToProvisionRestoration.Contains(data) == false)
                    {
                        timer.shopsToProvisionRestoration.Add(data);
                    }
                }
                squad.data.inventory.Add((Item)saleable);
                data.goods.Remove(saleable);
                Show(data);
                OpenInventoryInSquad();
            }
            else if(saleable is Card && squad.data.cards.Count < 10)
            {
                squad.data.gold -= cost;
                data.money += cost;
                sounder.PlayOneShot(money[Random.Range(0, money.Length)]);
                squad.data.cards.Add((Card)saleable);
                data.goods.Remove(saleable);
                Show(data);
                OpenCardsInSquad();
            }

            foreach (Transform child in itemPanel.transform)
                child.gameObject.SetActive(false);
            foreach (Transform child in cardPanel.transform)
                child.gameObject.SetActive(false);
        }
    }
    public void OpenInventoryInSquad()
    {
        squad.DisplayResourcesCount();
        squad.DisplayItems();
        squad.inventoryMode = true;
        squad.changePanelModeButton.sprite = squad.cardsIcon;
    }
    public void OpenCardsInSquad()
    {
        squad.DisplayResourcesCount();
        squad.DisplaySquadCards();
        squad.inventoryMode = false;
        squad.changePanelModeButton.sprite = squad.inventoryIcon;
    }
    public void ClearContent()
    {
        foreach (Transform child in content)
        {
            Destroy(child.gameObject);
        }
    }
    public static ShopData GetTownShop(ItemsAndClassesInfo info)
    {
        List<ISaleable> goods = new List<ISaleable>();
        for (int i = 0; i < Random.Range(1, 5); i++)
        {
            int weaponIndex = Random.Range(0, 7);
            goods.Add(new Weapon(weaponIndex, (WeaponPattern)info.items[weaponIndex]));
        }
        for (int i = 0; i < Random.Range(1, 5); i++)
        {
            int armorIndex = Random.Range(8, 11);
            goods.Add(new Armor(armorIndex, (ArmorPattern)info.items[armorIndex]));
        }
        for (int i = 0; i < Random.Range(3, 8); i++)
        {
            goods.Add(new Hill(11, (HillPattern)info.items[11]));
        }
        for (int i = 0; i < Random.Range(2, 4); i++)
        {
            goods.Add(new Hill(15, (HillPattern)info.items[15]));
        }
        for (int i = 0; i < Random.Range(8, 15); i++)
        {
            goods.Add(new Item(12, info.items[12].cost));
        }
        ShopData res = new ShopData(goods, Random.Range(300, 500), ShopType.ItemsShop, 1.4f, ShopName.TownShop);
        return res;
    }

    public static ShopData GetVillageShop(ItemsAndClassesInfo info)
    {
        List<ISaleable> goods = new List<ISaleable>();
        for(int i = 0; i < Random.Range(1, 5); i++)
        {
            int weaponIndex = Random.Range(0, 7);
            goods.Add(new Weapon(weaponIndex, (WeaponPattern)info.items[weaponIndex]));
        }
        for (int i = 0; i < Random.Range(1, 5); i++)
        {
            int armorIndex = Random.Range(8, 11);
            goods.Add(new Armor(armorIndex, (ArmorPattern)info.items[armorIndex]));
        }
        for (int i = 0; i < Random.Range(1, 4); i++)
        {
            goods.Add(new Hill(11, (HillPattern)info.items[11]));
        }
        for (int i = 0; i < Random.Range(0, 2); i++)
        {
            goods.Add(new Hill(15, (HillPattern)info.items[15]));
        }
        for (int i = 0; i < Random.Range(3, 8); i++)
        {
            goods.Add(new Item(12, info.items[12].cost));
        }
        ShopData res = new ShopData(goods, Random.Range(100, 200), ShopType.ItemsShop, 1.2f, ShopName.VillageShop);
        return res;
    }

    public static ShopData GetTavernShop(ItemsAndClassesInfo info)
    {
        List<ISaleable> goods = new List<ISaleable>();
        for(int i = 0; i < Random.Range(2, 5); i++)
        {
            goods.Add(Squad.CreateRandomNewMercenary(info));
        }
        ShopData res = new ShopData(goods, 500, ShopType.CardsShop, 1.2f, ShopName.Tavern);
        return res;
    }
}
public interface ISaleable
{
    public int Price { get; }
}


[System.Serializable]
public class ShopData
{
    public List<ISaleable> goods;
    public int money;
    public ShopType type;
    public float priceDistortion;

    public ShopName name;

    public int GetCountOf(int itemID)
    {
        int res = 0;
        foreach(ISaleable good in goods)
        {
            if(good is Item && ((Item)good).patternID == itemID)
            {
                res++;
            }
        }
        return res;
    }

    public ShopData(List<ISaleable> goods, int money, ShopType type, float priceDistortion, ShopName name)
    {
        this.goods = goods;
        this.money = money;
        this.type = type;
        this.priceDistortion = priceDistortion;
        this.name = name;
    }
    public string GetName()
    {
        string[] names = new string[] { "Деревенские торговцы", "Таверна", "Городской рынок"};
        return names[(int)name];
    }

    public static ShopData operator +(ShopData store1, ShopData store2)
    {
        var combinedProducts = new List<ISaleable>(store1.goods);
        combinedProducts.AddRange(store2.goods);

        var combinedMoney = store1.money + store2.money;

        return new ShopData(combinedProducts, combinedMoney, store1.type, store1.priceDistortion, store1.name);
    }
}
public enum ShopName
{
    VillageShop,
    Tavern,
    TownShop
}
public enum ShopType
{
    ItemsShop,
    CardsShop
}
