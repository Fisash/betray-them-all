using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class SquadMemberInfoPanel : MonoBehaviour
{
    public TextMeshProUGUI nameText;
    public TextMeshProUGUI descriptionText;
    public TextMeshProUGUI weaponDescription;
    public TextMeshProUGUI armorDescription;
    public TextMeshProUGUI priceText;

    public Image weaponImage;
    public Image armorImage;

    public Transform effectsContent;
    public GameObject effectBox;

    public ItemsAndClassesInfo info;

    public bool displayItems = true;

    public bool showPrice = false;

    public bool showEffects = true;

    public float priceDistortion = 1f;


    public void SetDisplay(Card card)
    {
        nameText.text = card.name;
        string typeName = (int)card.type < info.classes.Length ? info.classes[(int)card.type].name : info.enemyPatterns[(int)card.type - info.classes.Length].name;
        string description = "Класс: <color=#99D0FF>" + typeName + "</color>";
        description +=  "\nЗдоровье: <color=#fa5757>" + card.hp.ToString() + "/" + (card.maxHp+card.addingMaxHp).ToString() + "</color>";
        description += "\nУрон: <color=#f27b35>" + card.GetDamage().x.ToString() + "-" + card.GetDamage().y.ToString() + "</color>";
        description += "\nШанс КРТ удара: <color=#9cf3ff>" + card.GetCriticalStikeChance().ToString() + "%</color>";
        description += "\nЗащита: <color=#9e9178>" + card.GetProtection().x.ToString() + "-" + card.GetProtection().y.ToString() + "</color>";
        description += "\nШанс Уклонения: <color=#9e9178>" + card.GetEvasionChance().ToString() + "%</color>";
        description += "\nУровень <color=#f7e85e>" + card.level.ToString() + "</color> (Опыт: <color=#f7e85e>" + card.exp.ToString() + "/" + card.maxExp.ToString() + "</color>)";
        descriptionText.text = description;
        
        if (displayItems)
        {
            WeaponPattern wPattern = info.GetWeaponPatternByEnam(card.weapon.type);
            Sprite weaponIcon = wPattern.icon;
            string weapon = wPattern.title + ".\n" + wPattern.GetDescription(card.weapon);
            weaponDescription.text = weapon;
            weaponImage.sprite = weaponIcon;


            ArmorPattern aPattern = info.GetArmorPatternByEnam(card.armor.type);
            Sprite armorIcon = aPattern.icon;
            string armor = aPattern.title + ".\n" + aPattern.GetDescription(card.armor);
            armorDescription.text = armor;
            armorImage.sprite = armorIcon;
        }

        effectsContent.gameObject.SetActive(false);
        if (showEffects)
        {
            effectsContent.gameObject.SetActive(card.effects.Count > 0);

            foreach (Transform child in effectsContent)
            {
                Destroy(child.gameObject);
            }

            foreach (Effect effect in card.effects)
            {
                GameObject box = Instantiate(effectBox, effectsContent);
                EffectInfo effInfo = info.effects[effect.id];
                string effectText = effInfo.title + ": " + effInfo.description + "\nЗакончится через: ";
                switch (effect.durationType)
                {
                    case EffectTypeByDuration.ExpiringDays:
                        effectText += effect.expirationTimeNumber.ToString() + " дней.";
                        break;
                    case EffectTypeByDuration.ExpiringMoves:
                        effectText += (effect.expirationTimeNumber+1).ToString() + " ходов боя.";
                        break;
                    case EffectTypeByDuration.Permanent:
                        effectText += " никогда.";
                        break;
                }
                box.transform.GetChild(0).GetComponent<TextMeshProUGUI>().text = effectText;
            }
        }

        if (showPrice)
        {

            int cost = Mathf.RoundToInt(card.Price * priceDistortion);
            priceText.text = "Цена: " + cost.ToString();
        }
    }
}
