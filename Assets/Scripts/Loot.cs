using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Loot : MonoBehaviour
{

    public EventController eventer;

    public GameObject lootPanel;

    public Transform lootContent;
    public GameObject itemPrefab;

    public LootData data;

    public TextMeshProUGUI lootName;

    public void ClearContent()
    {
        foreach (Transform child in lootContent)
        {
            Destroy(child.gameObject);
        }
    }
    public void ClosePanel()
    {
        data = null;
        lootPanel.SetActive(false);
    }
    public void ShowLoot()
    {
        lootName.text = data.name;
        lootPanel.SetActive(true);
        ClearContent();
        foreach (Item item in data.items)
        {
            GameObject newItemPrefab = Instantiate(itemPrefab, lootContent);
            ItemPrefab slot = newItemPrefab.GetComponent<ItemPrefab>();
            slot.SetType(ItemPrefabType.LootItem);
            slot.panelName = "lootItemHint";
            slot.SetDisplay(item);
        }
        eventer.AddEventToQueue(eventer.GetEventByID("loot"));
    }
    public static LootData GetBanditCampLoot(ItemsAndClassesInfo info)
    {
        List<Item> items = new();

        for (int i = 0; i < Random.Range(0, 3); i++)
        {
            int weaponIndex = Random.Range(0, 7);
            items.Add(new Weapon(weaponIndex, (WeaponPattern)info.items[weaponIndex]));
        }
        for (int i = 0; i < Random.Range(0, 3); i++)
        {
            int armorIndex = Random.Range(8, 11);
            items.Add(new Armor(armorIndex, (ArmorPattern)info.items[armorIndex]));
        }
        for (int i = 0; i < Random.Range(1, 3); i++)
        {
            items.Add(new Hill(11, (HillPattern)info.items[11]));
        }
        return new LootData("Лагерь разбойников", items);
    }
    public static LootData GetCaveLoot(ItemsAndClassesInfo info)
    {
        List<Item> items = new();

        for (int i = 0; i < Random.Range(2, 5); i++)
        {
            int weaponIndex = Random.Range(0, 7);
            items.Add(new Weapon(weaponIndex, (WeaponPattern)info.items[weaponIndex]));
        }
        for (int i = 0; i < Random.Range(2, 5); i++)
        {
            int armorIndex = Random.Range(8, 11);
            items.Add(new Armor(armorIndex, (ArmorPattern)info.items[armorIndex]));
        }
        for (int i = 0; i < Random.Range(2, 5); i++)
        {
            items.Add(new Hill(11, (HillPattern)info.items[11]));
        }
        return new LootData("Сокровище пещеры", items);
    }
    public static LootData GetGriffinNestLoot(ItemsAndClassesInfo info)
    {
        List<Item> items = new();

        for (int i = 0; i < Random.Range(1, 5); i++)
        {
            items.Add(new Item(14, info.items[14].cost));
        }
        return new LootData("Гнездо грифонов", items);
    }
    public static LootData GetVampireLoot(ItemsAndClassesInfo info)
    {
        List<Item> items = new();

       
        int weaponIndex = Random.Range(0, 7);
        Weapon weapon = new Weapon(weaponIndex, (WeaponPattern)info.items[weaponIndex]);
        weapon.damage += Random.Range(5, 10);
        weapon.criticalStrikeChance += Random.Range(3, 8);
        items.Add(weapon);
        
        
        int armorIndex = Random.Range(8, 11);
        Armor armor = new Armor(armorIndex, (ArmorPattern)info.items[armorIndex]);
        armor.protection += Random.Range(3, 8);
        armor.evasionChance += Random.Range(3, 5);
        items.Add(armor);
        
        for (int i = 0; i < Random.Range(1, 3); i++)
        {
            items.Add(new Hill(11, (HillPattern)info.items[11]));
        }
        for (int i = 0; i < Random.Range(2, 5); i++)
        {
            items.Add(new Item(12, info.items[12].cost));
        }
        return new LootData("Особняк вампиров", items);
    }
    public static LootData GetSwordmanLoot(ItemsAndClassesInfo info)
    {
        List<Item> items = new();
        Weapon weapon = new Weapon(0, (WeaponPattern)info.items[0]);
        weapon.damage = Random.Range(15, 20);
        weapon.criticalStrikeChance = Random.Range(8, 13);
        items.Add(weapon);
        return new LootData("Безумный мечник", items);
    }
}

[System.Serializable]
public class LootData
{
    public string name;
    public List<Item> items;

    public LootData(string name, List<Item> items)
    {
        this.name = name;
        this.items = items;
    }

    public static LootData operator +(LootData loot1, LootData loot2)
    {
        var combinedProducts = new List<Item>(loot1.items);
        combinedProducts.AddRange(loot2.items);
        return new LootData(loot1.name, combinedProducts) ;
    }
}
