using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using TMPro;

public class SkillSlot : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{

    Skill skill;
    Image icon;

    Fight fight;

    bool isActive;

    void Awake()
    {
        icon = GetComponent<Image>();
        fight = GameObject.Find("Fight").GetComponent<Fight>();
    }
    public void UpdateDisplay(Skill skill)
    {
        this.skill = skill;
        icon.sprite = skill.icon;
    }
    public void SetBacklight(bool active)
    {
        Color notSuit = new Color(1f, 1f, 1f, 0.5f);
        Color suit = new Color(1f, 1f, 1f, 1f);
        isActive = active;
        icon.color = (active) ? suit : notSuit;
    }
    public void TapSkill()
    {
        if (!isActive)
        {
            return;
        }
        fight.TapSkill(skill);
    }
    public void OnPointerEnter(PointerEventData eventData)
    {
        fight.ShowSkillHint(skill);
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        fight.HideSkillHint();
    }
}
