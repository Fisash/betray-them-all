using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public interface ITapItemHandler
{
    public void TapItem(Item item);
}
public enum ItemPrefabType
{
    SquadItem,
    ShopItem,
    FightItem,
    LootItem
}
public class ItemPrefab : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    public Item item;

    public Image icon;

    public GameObject infoPanel;
    public ItemHintPanel infoPanelUpdater;

    public ItemsAndClassesInfo info;
    ITapItemHandler tapHandler;
    public ItemPrefabType type = ItemPrefabType.SquadItem;

    public string panelName = "itemHint";

    public void SetType(ItemPrefabType type)
    {
        this.type = type;
        switch (type)
        {
            case ItemPrefabType.SquadItem:
                tapHandler = GameObject.Find("squad").GetComponent<Squad>();
                break;
            case ItemPrefabType.ShopItem:
                tapHandler = GameObject.Find("Shop").GetComponent<Shop>();
                break;
            case ItemPrefabType.FightItem:
                tapHandler = GameObject.Find("Fight").GetComponent<Fight>();
                break;
            case ItemPrefabType.LootItem:
                tapHandler = null;
                break;
        }
    }
    public void SetDisplay(Item item)
    {
        infoPanel = GameObject.Find(panelName);
        infoPanelUpdater = infoPanel.GetComponent<ItemHintPanel>();

        this.item = item;
        icon.sprite = info.items[item.patternID].icon;
    }

    public void PressMe()
    {
        if(tapHandler != null)
            tapHandler.TapItem(item);
    }
    public void OnPointerEnter(PointerEventData eventData)
    {
        infoPanelUpdater.SetDisplay(item);
        foreach (Transform child in infoPanel.transform)
        {
            child.gameObject.SetActive(true);
        }
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        foreach (Transform child in infoPanel.transform)
        {
            child.gameObject.SetActive(false);
        }
    }
}
