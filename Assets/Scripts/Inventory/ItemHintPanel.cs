using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class ItemHintPanel : MonoBehaviour
{
    public TextMeshProUGUI nameText;
    public TextMeshProUGUI descriptionText;

    public TextMeshProUGUI costText;
    public bool isShowCost = false;

    public ItemsAndClassesInfo info;
    public float priceDistortion = 1f;

    public void SetDisplay(Item item)
    {
        ItemPattern pattern = info.items[item.patternID];
        nameText.text = pattern.title;
        descriptionText.text = pattern.GetDescription(item);
        if (isShowCost)
        {
            int cost = Mathf.RoundToInt(item.Price * priceDistortion);
            costText.text = "Цена: " + cost.ToString();
        }
    }
}
