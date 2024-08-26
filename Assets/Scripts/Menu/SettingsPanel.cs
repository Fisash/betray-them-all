using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class SettingsPanel : MonoBehaviour
{
    public GameObject settingsPanel;
    public TextMeshProUGUI avoidingMountains;
    public TextMeshProUGUI avoidingForests;
    public Scrollbar sound;
    public Scrollbar music;

    SettingsData data;

    private void Start()
    {
        sound.onValueChanged.AddListener(value => ChangeScrollbar(0, sound.value));
        music.onValueChanged.AddListener(value => ChangeScrollbar(1, music.value));
    }
    public void Load(SettingsData data)
    {
        avoidingMountains.text = "Избегать горы (" + (data.isAvoidgMountains ? "Да)" : "Нет)");
        avoidingForests.text = "Избегать лес (" + (data.isAvoidgForests ? "Да)" : "Нет)");

        sound.value = (data.soundVolume / 100f);
        music.value = (data.musicVolume / 100f);

        this.data = data;
    }

    public void ChangeActive() => settingsPanel.SetActive(!settingsPanel.activeSelf);

    public void TapButton(int id)
    {
        switch (id)
        {
            case 0:
                data.isAvoidgMountains = !data.isAvoidgMountains;
                break;
            case 1:
                data.isAvoidgForests = !data.isAvoidgForests;
                break;
        }
        Load(data);
    }

    public void ChangeScrollbar(int id, float value)
    {
        switch (id)
        {
            case 0:
                data.soundVolume = Mathf.RoundToInt(value * 100f);
                break;
            case 1:
                data.musicVolume = Mathf.RoundToInt(value * 100f);
                break;
        }
        Load(data);
    }

    public void Save()
    {
        GameObject.Find("Canvas").GetComponent<Settings>().SaveSettings(data);
    }

}