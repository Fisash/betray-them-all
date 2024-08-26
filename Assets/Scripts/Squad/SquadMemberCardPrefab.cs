using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using TMPro;

public interface ITapCardHandler
{
    public void TapCard(Card card);
}

public class SquadMemberCardPrefab : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    public Card card;

    public Image icon;
    public TextMeshProUGUI hpText;
    public TextMeshProUGUI levelText;

    GameObject infoPanel;
    SquadMemberInfoPanel infoPanelUpdater;

    ITapCardHandler tapHandler;
    public ItemPrefabType type;

    public ItemsAndClassesInfo info;

    public string panelName = "squadMemberHint";

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
        }

    }
    public void PressMe()
    {
        tapHandler.TapCard(card);
    }
    public void SetDisplay(Card card)
    {
        infoPanel = GameObject.Find(panelName);
        infoPanelUpdater = infoPanel.GetComponent<SquadMemberInfoPanel>();

        this.card = card;
        icon.sprite = info.GetClassPatternByType(card.type).icnons[card.iconID];
        hpText.text = card.hp.ToString() + "/" + (card.maxHp+card.addingMaxHp).ToString();
        levelText.text = card.level.ToString();
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        infoPanelUpdater.SetDisplay(card);
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
