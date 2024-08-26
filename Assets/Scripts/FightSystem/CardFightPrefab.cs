using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using TMPro;

public class CardFightPrefab : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{

    public Card card = null;

    public int index;
    public bool isEnemy;

    TextMeshProUGUI hpText;
    TextMeshProUGUI levelText;
    Image icon;

    Fight fight;
    public ItemsAndClassesInfo info;

    GameObject squadMemberHint;
    SquadMemberInfoPanel squanMemberPanel;

    GameObject enemyHint;
    SquadMemberInfoPanel enemyPanel;
    Outline outline;

    public Animator animator;

    bool isActive = true;

    void Awake()
    {
        squadMemberHint = GameObject.Find("squadMemberHint");
        squanMemberPanel = squadMemberHint.GetComponent<SquadMemberInfoPanel>();

        enemyHint = GameObject.Find("enemyHint");
        enemyPanel = enemyHint.GetComponent<SquadMemberInfoPanel>();

        outline = GetComponent<Outline>();

        hpText = transform.GetChild(0).GetComponent<TextMeshProUGUI>();
        levelText = transform.GetChild(1).GetComponent<TextMeshProUGUI>();
        icon = GetComponent<Image>();
        fight = GameObject.Find("Fight").GetComponent<Fight>();
    }

    public void SetBacklight(bool active)
    {
        Color notSuit = new Color(1f, 1f, 1f, 0.5f);
        Color suit = new Color(1f, 1f, 1f, 1f);
        isActive = active;
        icon.color = (active) ? suit : notSuit;
    }
    public void UpdateDisplay()
    {
        hpText.gameObject.SetActive(true);
        levelText.gameObject.SetActive(true);
        card = isEnemy ? fight.data.enemyCards[index] : fight.data.playerCards[index];
        if(card == null || !card.isNotEmpty)
        {
            icon.sprite = fight.emptyCardIcon;
            hpText.gameObject.SetActive(false);
            levelText.gameObject.SetActive(false);
            return;
        }
        icon.sprite = info.GetClassPatternByType(card.type).icnons[card.iconID];
        hpText.text = card.hp.ToString() + "/" + (card.maxHp+card.addingMaxHp).ToString();
        levelText.text = card.level.ToString();
    }
    public void TapCard()
    {
        if (isActive)
        {
            fight.TapCard(this);
        }
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        if(card == null || !card.isNotEmpty)
        {
            return;
        }
        if (isActive)
        {
            outline.enabled = true;
        }
        SquadMemberInfoPanel panel = isEnemy ? enemyPanel : squanMemberPanel;
        panel.SetDisplay(card);
        foreach (Transform child in isEnemy ? enemyHint.transform : squadMemberHint.transform)
        {
            child.gameObject.SetActive(true);
        }
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        outline.enabled = false;
        foreach (Transform child in squadMemberHint.transform)
        {
            child.gameObject.SetActive(false);
        }
        foreach (Transform child in enemyHint.transform)
        {
            child.gameObject.SetActive(false);
        }
    }
}
